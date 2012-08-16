#ifndef SIG_GEN_FACTORY_HPP
#define SIG_GEN_FACTORY_HPP

#include <map>
#include <memory>
#include <iostream>
#include <stdexcept>


struct SigGenFactory
{
  typedef ISigGenDefinition* (*CreateRuleCallback)();
  typedef std::map<std::string, CreateRuleCallback> SigGenMap;
  SigGenMap iMap_;
  static SigGenFactory* pInstance_;
  static bool destroyed_;

  //disable CTOR and copy, and assignment capabilities
  SigGenFactory(){};
  SigGenFactory(const SigGenFactory&);
  SigGenFactory& operator=(const SigGenFactory&);

  virtual ~SigGenFactory(){
     pInstance_=0;
     destroyed_=true;
  }

   public:

  //create single instance
  static SigGenFactory& Instance(){
     if(!pInstance_){
        if(destroyed_) throw std::runtime_error("Dead reference detected");
        else{
           static SigGenFactory obj;
           pInstance_ = &obj;
        }
     }
     return *pInstance_;
  }

  //Every class must register its callback here
  const bool RegisterSigGen(const std::string& id , CreateRuleCallback createFun){
     SigGenMap::iterator iter = iMap_.find(id);
     return iMap_.insert( SigGenMap::value_type(id,createFun)).second;
  }

  const bool UnregisterSigGen(const std::string& id){
     return iMap_.erase(id)==1;
  }

  //Return object of specified type or throw exception
  ISigGenDefinition* Create(const std::string& id){
     SigGenMap::const_iterator iter = iMap_.find(id);
     if(iter == iMap_.end()){
        throw std::runtime_error("SigGenFactory - Unknown Rule ID " + id);
     }
     return (iter->second)();
  }

   private:
};


#endif
