#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "G4THitsMap.hh"
#include <vector>

class G4Run;
class G4Event;

class Run : public G4Run
{
    public:
        Run(const std::vector<G4String> mfdName);
        virtual ~Run();

        void RecordEvent(const G4Event*);
        void Merge(const G4Run*);

    public:
        G4int GetNumberOfHitsMap() const {return fRunMap.size();}

        G4THitsMap<G4double>* GetHitsMap(G4int i){return fRunMap[i];}
        G4THitsMap<G4double>* GetHitsMap(const G4String& detName, const G4String& colName);
        G4THitsMap<G4double>* GetHitsMap(const G4String& fullName);

        // Dump All HitsMap of this RUN.
        void DumpAllScorer();

    private:
        std::vector<G4String> fCollName;
        std::vector<G4int> fCollID;
        std::vector<G4THitsMap<G4double>*> fRunMap;
};
#endif


