#include "acequia_manager.h"
#include <iostream>
#include <vector>

void solveProblems(AcequiaManager& manager)
{
    // Get the vectors only once
    auto canals = manager.getCanals();
    
    // Max flow rate is 1.0 gal/s
    const double MAX_FLOW = 1.0;

    // The loop continues until all regions are solved or max time is reached
    while(!manager.isSolved && manager.hour != manager.SimulationMax)
    {
        // Iterate through all canals to decide the state of each one for this hour.
        for (const auto& canal : canals)
        {
            // Get pointers to the connected regions for cleaner code
            Region* source = canal->sourceRegion;
            Region* dest = canal->destinationRegion;
            
            // --- Determine if the canal should be OPEN ---
            
            // Condition 1: Destination needs water (Level < Need OR in Drought)
            bool dest_needs_water = (dest->waterLevel < dest->waterNeed) || dest->isInDrought;

            // Condition 2: Source can spare water (Level > Need AND not in Drought)
            // A more conservative check: Source is above its need AND not flooded (avoid wasting water)
            bool source_has_surplus = (source->waterLevel > source->waterNeed) && !source->isFlooded;

            // Condition 3: Emergency opening if the source is flooded (draining priority)
            bool source_is_flooded = source->isFlooded;


            // OPEN LOGIC: If the destination needs water AND the source can provide it OR the source is flooded.
            if (dest_needs_water && source_has_surplus)
            {
                // Open at max flow.
                canal->setFlowRate(MAX_FLOW);
                canal->toggleOpen(true);
            }
            // Secondary OPEN LOGIC: If the source is flooded, open to drain (unless destination is also flooded).
            else if (source_is_flooded && !dest->isFlooded)
            {
                canal->setFlowRate(MAX_FLOW);
                canal->toggleOpen(true);
            }

            // --- Determine if the canal should be CLOSED ---
            
            // Condition 4: Destination is solved (no longer needs this canal's flow)
            bool dest_is_solved = !dest->isFlooded && !dest->isInDrought && (dest->waterLevel >= dest->waterNeed);
            
            // Condition 5: Source is now in deficit (cannot afford to lose water)
            bool source_is_in_deficit = source->waterLevel <= source->waterNeed;


            // CLOSE LOGIC: If the destination is solved OR the source region is running low.
            if (dest_is_solved || source_is_in_deficit)
            {
                canal->toggleOpen(false);
            }
        }

        // Advance time for the simulation to execute all planned water transfers
        manager.nexthour();
    }
}
