#ifndef NODE_FACTORY_HPP
#define NODE_FACTORY_HPP

#include<iostream>
#include<boost/function.hpp>
#include<boost/shared_ptr.hpp>
#include<sdr_simulator/yaml/ConfigNode.hpp>


namespace yaml
{
	typedef ConfigNodePtr(*NodeCallback)();
   typedef std::map<std::string,NodeCallback> ConfigNodeMap;

	struct NodeFactory
	{
		static NodeFactory* pInstance_;
		static bool destroyed_;
		ConfigNodeMap nodeMap_;

		//disable CTOR and copy, and assignment capabilities
		NodeFactory(){};
		NodeFactory(const NodeFactory&);
		NodeFactory& operator=(const NodeFactory&);

		virtual ~NodeFactory()
		{
			pInstance_=0;
			destroyed_=true;
		}

		public:

		static NodeFactory& Instance()
		{
			if(!pInstance_)
			{
				if(destroyed_) throw std::runtime_error("NodeFactory::Instance Dead Reference Detected");
				static NodeFactory obj;
				pInstance_ = &obj;
			}

			return *pInstance_;
		}

		bool UnRegister(const std::string& name) { nodeMap_.erase(name)==1; }
		bool Register(const std::string& name, NodeCallback f ) {
         std::cout << "Register " << name << std::endl;
         nodeMap_.insert( ConfigNodeMap::value_type(name,f)).second;}

		ConfigNodePtr Create(const std::string& name) { 
			ConfigNodeMap::const_iterator iter = nodeMap_.find(name);
			if(iter == nodeMap_.end()){
				throw std::runtime_error("NodeFactory::Create : Unknown Node Type " + name);
			}
			return (iter->second)();
		}
	};

};

#endif
