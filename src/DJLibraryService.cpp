#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

//Added destrcutor 
DJLibraryService::~DJLibraryService() {
    for(AudioTrack* track : library){
        delete track;
    }
    library.clear();
}
    /**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    int count = 0;
    for (size_t i = 0; i < library_tracks.size(); ++i) {
        AudioTrack* newTrack = nullptr;
        if(library_tracks[i].type == "MP3"){ 
            newTrack = new MP3Track(
                library_tracks[i].title,
                library_tracks[i].artists, 
                library_tracks[i].duration_seconds, 
                library_tracks[i].bpm, 
                library_tracks[i].extra_param1, 
                library_tracks[i].extra_param2 != 0);
            library.push_back(newTrack);
            count++;
        }
        else if(library_tracks[i].type == "WAV"){
            newTrack = new WAVTrack(
                library_tracks[i].title,
                library_tracks[i].artists, 
                library_tracks[i].duration_seconds, 
                library_tracks[i].bpm, 
                library_tracks[i].extra_param1, 
                library_tracks[i].extra_param2);
            library.push_back(newTrack);
            count++;
        }
    }
    std::cout << "[INFO] Track library built: " << count << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    int librarySize = library.size();
    for(int index: track_indices){
        if(index < 1 || index > librarySize){
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
        }
        else{
            AudioTrack* clonedTrack = library[index-1]->clone().release();
            if(clonedTrack == nullptr){
                std::cout << "[ERROR] Cloned track from index: " << index << " is nullptr" << std::endl;
            }
            else{
                clonedTrack->load();
                clonedTrack->analyze_beatgrid();
                playlist.add_track(clonedTrack);
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist.get_name() << " ("<< playlist.get_track_count() << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for (size_t i = tracks.size(); i > 0; --i) {
        AudioTrack* track = tracks[i - 1];
        if (track) {
            titles.push_back(track->get_title());
        }
    }
    return titles;
}
