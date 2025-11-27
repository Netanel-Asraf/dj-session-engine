#include "Playlist.h"
#include "AudioTrack.h"
#include <iostream>
#include <algorithm>
Playlist::Playlist(const std::string& name) 
    : head(nullptr), playlist_name(name), track_count(0) {
    std::cout << "Created playlist: " << name << std::endl;
}
// TODO: Fix memory leaks!
// Students must fix this in Phase 1
Playlist::~Playlist() {
    #ifdef DEBUG
    std::cout << "Destroying playlist: " << playlist_name << std::endl;
    #endif
    PlaylistNode* cur = head;
    while(cur) {
        PlaylistNode* nxt = cur->next;
        delete cur->track;
        delete cur;
        cur = nxt;   
    }
    head = nullptr;
}

Playlist::Playlist(const Playlist& other)
    :   head(nullptr),
        playlist_name(other.playlist_name),
        track_count(other.track_count)
    {
        // TODO: Implement the copy constructor
        #ifdef DEBUG
        std::cout << "Playlist copy constructor called for: " << other.playlist_name << std::endl;
        #endif
        if (other.head != nullptr){
            PlaylistNode* otherCur = other.head;
            PlaylistNode* myTail = nullptr;
            while (otherCur!=nullptr){
                AudioTrack* newTrack = otherCur->track->clone().release();
                if (newTrack) {
                    // 2. Create new node
                    PlaylistNode* newNode = new PlaylistNode(newTrack);
                    // 3. Append to list
                    if (head == nullptr) 
                    {
                        head = newNode;
                        myTail = newNode;
                    } 
                    else 
                    {
                        myTail->next = newNode;
                        myTail = newNode;
                    }
                    track_count++;
                    }
                otherCur = otherCur->next;
                }
        }
    }

// Playlist& Playlist::operator=(const Playlist& other) {
//     // TODO: Implement the copy assignment operator
//     #ifdef DEBUG
//     std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
//     #endif
//     // Your code here...
//     if(this != &other){
//         if(this->waveform_data != nullptr) {
//             delete[] this->waveform_data;
//         }
//         title = other.title;
//         artists = other.artists;
//         duration_seconds = other.duration_seconds;
//         bpm = other.bpm;
//         waveform_size = other.waveform_size;

//         if(other.waveform_data != nullptr){
//             waveform_data = new double[waveform_size];
//             for(size_t i = 0; i < other.waveform_size; i++){
//                 waveform_data[i] = other.waveform_data[i];
//             }
//         }
//         else{
//             waveform_data = nullptr;
//         }
//     }
//     return *this;
// }

void Playlist::add_track(AudioTrack* track) {
    if (!track) {
        std::cout << "[Error] Cannot add null track to playlist" << std::endl;
        return;
    }

    // Create new node - this allocates memory!
    PlaylistNode* new_node = new PlaylistNode(track);

    // Add to front of list
    new_node->next = head;
    head = new_node;
    track_count++;

    std::cout << "Added '" << track->get_title() << "' to playlist '" 
              << playlist_name << "'" << std::endl;
}

void Playlist::remove_track(const std::string& title) {
    PlaylistNode* current = head;
    PlaylistNode* prev = nullptr;

    // Find the track to remove
    while (current && current->track->get_title() != title) {
        prev = current;
        current = current->next;
    }

    if (current) {
        // Remove from linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }

        track_count--;
        std::cout << "Removed '" << title << "' from playlist" << std::endl;

    } else {
        std::cout << "Track '" << title << "' not found in playlist" << std::endl;
    }
    delete current->track;
    delete current;
}

void Playlist::display() const {
    std::cout << "\n=== Playlist: " << playlist_name << " ===" << std::endl;
    std::cout << "Track count: " << track_count << std::endl;

    PlaylistNode* current = head;
    int index = 1;

    while (current) {
        std::vector<std::string> artists = current->track->get_artists();
        std::string artist_list;

        std::for_each(artists.begin(), artists.end(), [&](const std::string& artist) {
            if (!artist_list.empty()) {
                artist_list += ", ";
            }
            artist_list += artist;
        });

        AudioTrack* track = current->track;
        std::cout << index << ". " << track->get_title() 
                  << " by " << artist_list
                  << " (" << track->get_duration() << "s, " 
                  << track->get_bpm() << " BPM)" << std::endl;
        current = current->next;
        index++;
    }

    if (track_count == 0) {
        std::cout << "(Empty playlist)" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}

AudioTrack* Playlist::find_track(const std::string& title) const {
    PlaylistNode* current = head;

    while (current) {
        if (current->track->get_title() == title) {
            return current->track;
        }
        current = current->next;
    }

    return nullptr;
}

int Playlist::get_total_duration() const {
    int total = 0;
    PlaylistNode* current = head;

    while (current) {
        total += current->track->get_duration();
        current = current->next;
    }

    return total;
}

std::vector<AudioTrack*> Playlist::getTracks() const {
    std::vector<AudioTrack*> tracks;
    PlaylistNode* current = head;
    while (current) {
        if (current->track)
            tracks.push_back(current->track);
        current = current->next;
    }
    return tracks;
}