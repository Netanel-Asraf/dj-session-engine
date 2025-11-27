#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks....\n";
    for (int i=0 ; i<=1 ; i++){
        if(decks[i] != nullptr){
        delete decks[i];
        decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {

    PointerWrapper<AudioTrack> tr;
    int target = 1 - active_deck;

    // First track load
    if(decks[0] == nullptr && decks[1] == nullptr){
        try{
            tr = track.clone();
        }
        catch(int errorCode){ 
            std::cout << "[ERROR] Track: " << track.get_title() <<" failed to clone\n";
            return -1;
        }
        decks[0] = tr.release();
    }

    // Subsequent track load
    else{
        std::cout << "\n=== Loading Track to Deck ===\n";
        PointerWrapper<AudioTrack> tr;
        try{
            tr = track.clone();
        }
        catch(int errorCode){ 
            std::cout << "[ERROR] Track: " << track.get_title() <<" failed to clone\n";
            return -1;
        }

        std::cout << "[Deck Switch] Target deck: " << target << "\n";

        // Empty active deck
        if(decks[target] != nullptr){
            delete decks[target];
            decks[target] = nullptr;
        }

        // Track preperation
        tr.get()->load();
        tr.get()->analyze_beatgrid();

        // BPM Management
        if(decks[active_deck] != nullptr && auto_sync)
            if(can_mix_tracks(tr))
                sync_bpm(tr);

        // Release target deck
        if(decks[target] != nullptr)
            delete decks[target];
        decks[target] = tr.release();
        std::cout << "[Load Complete] ’" << decks[target] -> get_title() << "’ is now loaded on deck " << target << "\n";

        // Instant unload
        if(decks[active_deck] != nullptr){
            std::cout << "[Unload] Unloading previous deck <" << active_deck << "> (<" << decks[active_deck]->get_title() << ">)\n";
            delete decks[active_deck];
            decks[active_deck] = nullptr;
        }

        active_deck = target;
        return active_deck;
    }
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
    }
}
