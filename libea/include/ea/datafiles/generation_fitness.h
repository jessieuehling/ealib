/* generation_fitness.h
 * 
 * This file is part of EALib.
 * 
 * Copyright 2012 David B. Knoester.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _EA_DATAFILES_GENERATION_FITNESS_H_
#define _EA_DATAFILES_GENERATION_FITNESS_H_

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <ea/datafile.h>
#include <ea/interface.h>

namespace ea {
    namespace datafiles {
        
        /*! Datafile for mean generation, and mean & max fitness.
         */
        template <typename EA>
        struct generation_fitness : record_statistics_event<EA> {
            generation_fitness(EA& ea) : record_statistics_event<EA>(ea), _df("fitness.dat") {
                _df.add_field("update")
                .add_field("mean_generation")
                .add_field("mean_fitness")
                .add_field("max_fitness");
            }
            
            virtual ~generation_fitness() {
            }
            
            virtual void operator()(EA& ea) {
                using namespace boost::accumulators;
                accumulator_set<double, stats<tag::mean> > gen;
                accumulator_set<double, stats<tag::mean, tag::max> > fit;
                
                for(typename EA::population_type::iterator i=ea.population().begin(); i!=ea.population().end(); ++i) {
                    gen(ind(i,ea).generation());                
                    fit(static_cast<double>(ind(i,ea).fitness()));
                }
                
                _df.write(ea.current_update())
                .write(mean(gen))
                .write(mean(fit))
                .write(max(fit))
                .endl();
            }
            
            datafile _df;
        };
        
    } // datafiles
} // ea

#endif
