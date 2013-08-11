// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of Software Defined Radar Simulator (SDRS) Software.
//
// SDRS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// SDRS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SDRS.  If not, see <http://www.gnu.org/licenses/>.
#include <vector>
#include <boost/math/special_functions/binomial.hpp>
#include <tr1/math.h>
#include <numeric>

using namespace boost::math::policies;

class CicErrorAnalysis
{

   enum { MEAN, VAR};

   typedef boost::math::policies::policy< 
      //domain_error<ignore_error> 
      > c_policy;

   std::vector<double> GetError( 
         const unsigned int numStages, 
         const unsigned int maxDecimation, 
         const unsigned int diffDelay, 
         const unsigned int type
         ) 
   {

      // match paper notation
      const unsigned int RM = maxDecimation*diffDelay;
      const unsigned int N = numStages;

      double hSum = 0;
      double bin1 = 0.0;
      double bin2 = 0.0;
      double sum=0.0;

      // compute maximum j index
      const unsigned int J1 = numStages+1;
      const unsigned int J_MAX = 2*numStages+1;

      std::vector<double> error(J_MAX);

      for (unsigned int j=1; j<J_MAX; ++j)
      {
         sum=0.0;
         if( j < J1 )
         {
            const unsigned int K_MAX = (RM-1)*N+j-1;

            for ( unsigned int k=0; k<=K_MAX; ++k)
            {
               const unsigned int L_MAX = static_cast<int>(k/RM);

               hSum = 0.0;
               for(unsigned int l=0; l<=L_MAX; ++l )
               {
                  bin1 = 
                     boost::math::binomial_coefficient<double>(
                           static_cast<unsigned>(numStages), 
                           static_cast<unsigned>(l)
                           );  

                  bin2 = 
                     boost::math::binomial_coefficient<double>(
                           static_cast<unsigned>(numStages - j + k - RM*l),
                           static_cast<unsigned>(k - RM*l)
                           );

                  hSum += static_cast<double>(std::tr1::pow<int>(-1.0,l) * bin1 * bin2);
               }
               sum += type == MEAN ? hSum : hSum*hSum;
            }
         }
         else
         {
            const unsigned int K_MAX = 2*numStages + 1 - j;

            for ( unsigned int k=0; k<=K_MAX; ++k)
            {
               bin1 = 
                  boost::math::binomial_coefficient<double>(
                        static_cast<unsigned>(2*numStages+1-j), 
                        static_cast<unsigned>(k)
                        );  

               hSum = static_cast<double>(std::tr1::pow<int>(-1.0,k) * bin1);
               sum += type == MEAN ? hSum : hSum*hSum;
            }
         }

         error[j-1] = sum;
      }

      return error;
   }

   public:

   const std::vector<int> ComputeBitWidths( 
         const unsigned int numStages,
			const unsigned int minDecimation,
         const unsigned int maxDecimation, 
         const unsigned int diffDelay, 
         const unsigned int numInputBits,
         const unsigned int numOutputBits 
         ) 
	{

		// Bit growth.
		double max_gain = numStages * std::tr1::log2( maxDecimation*diffDelay );
		double min_gain = numStages * std::tr1::log2( minDecimation*diffDelay );

		// MSB of maximum register width size.
		long bit_max = std::tr1::ceil( numInputBits + max_gain - 1 );
		long bit_min = std::tr1::ceil( numInputBits + min_gain - 1 );
		long bit_rem = bit_max - bit_min;

		// calculate total noise from bit removal 
		double total_std = std::tr1::pow(2.0,bit_rem)/std::tr1::sqrt(12.0);

		std::cout 
			<< "bit max     = " << bit_max  << "\n"
			<< "bit gain    = " << max_gain << "\n"
			<< "bit discard = " << bit_rem  << "\n"
			<< std::endl;

		const std::vector<double> mu = GetError( 
				numStages, 
				minDecimation, 
				diffDelay,
				MEAN
				);

		const std::vector<double> var = GetError( 
				numStages, 
				minDecimation, 
				diffDelay,
				VAR
				);

		std::vector<int> discard(2*numStages+1,0);

		for( int i=0; i<2*numStages; ++i)
		{
			discard[i] = static_cast<int>(-0.5*std::tr1::log2(var[i]) + std::tr1::log2(total_std) + 
					std::tr1::log2(6.0/numStages)/2.0);
			std::cout 
				 << "discard[" << i << "] = " << discard[i] << "\n"
				 << "var["     << i << "] = " << var[i]     << "\n"
				 << "tstd["    << i << "] = " << total_std
				<< std::endl;
		}

		discard[2*numStages] = bit_max - bit_min;

		for( int i=0; i<discard.size(); ++i)
		{
			discard[i] = bit_max - discard[i] + 1;
		}

		return discard;
	};
};
