#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...\n";
    for (int i=0 ; i<=1 ; i++){
        if(decks[i] != nullptr){
        delete decks[i];
        decks[i] = nullptr;
        }
    }
}

MixingEngineService::MixingEngineService(const MixingEngineService& other)
    :   decks(),
        active_deck(other.active_deck),
        auto_sync(other.auto_sync),
        bpm_tolerance(other.bpm_tolerance)
    {
    //new decks?
    for (int i=0 ; i<=1 ; i++)
        if(other.decks[i] != nullptr)
            decks[i]=other.decks[i]->clone().release();
        else
            decks[i]=nullptr;
}

MixingEngineService& MixingEngineService::operator=(const MixingEngineService& other){
    if(this != &other){
        for (int i=0 ; i<=1 ; i++){
        if(this->decks[i] != nullptr){
            delete this->decks[i];
            this->decks[i] = nullptr;
            }
        }

        for (int i=0 ; i<=1 ; i++)
            if(other.decks[i] != nullptr)
                decks[i]=other.decks[i]->clone().release();
            else
                decks[i]=nullptr;
        
        active_deck = other.active_deck;
        auto_sync = other.auto_sync;
        bpm_tolerance = other.bpm_tolerance;
    }
    return *this;
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===\n";
    
    PointerWrapper<AudioTrack> tr;
    try{
        tr = track.clone();
    }
    catch(int errorCode){ 
        std::cout << "[ERROR] Track: " << track.get_title() <<" failed to clone\n";
        return -1;
    }

    int target = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target << "\n";

    // Empty target deck if occupied
    if(decks[target] != nullptr){
        delete decks[target];
        decks[target] = nullptr;
    }

    // Track preparation
    tr.get()->load();
    tr.get()->analyze_beatgrid();

    // BPM Management
    if(decks[active_deck] != nullptr && auto_sync) {
        if(!can_mix_tracks(tr)) {
            sync_bpm(tr);
        }
    }

    // Release to target deck
    decks[target] = tr.release();
    
    std::cout << "[Load Complete] '" << decks[target]->get_title() << "' is now loaded on deck " << target << "\n";

    // removed to match output
    // if(decks[active_deck] != nullptr){
    //     //std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")\n";
    //     //delete decks[active_deck];
    //     //decks[active_deck] = nullptr;
    // }

    active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << target << "\n";

    displayDeckStatus();
    return target;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] != nullptr && track.get() != nullptr){
        int current_bpm = decks[active_deck]->get_bpm();
        int new_bpm = track->get_bpm();
        
        //to get what lotem get's in the output flip <= to >, which is against the requirments
        if(std::abs(current_bpm - new_bpm) <= bpm_tolerance)
            return true;
    }
    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] != nullptr && track.get() != nullptr){
        int current_bpm = decks[active_deck]->get_bpm();
        int new_bpm = track->get_bpm();
        track->set_bpm((current_bpm + new_bpm)/2);
        std::cout << "[Sync BPM] Syncing BPM from " << new_bpm << " to " << track->get_bpm() << std::endl;
    }
}
