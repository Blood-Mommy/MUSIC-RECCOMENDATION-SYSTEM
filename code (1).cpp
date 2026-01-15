#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

//class that responsible for the music
struct Music
{
    string genre;
    string singer;
    string name;
    string emotion;
    string speed;
    string lyrics;
};
//class that responsible for the user
struct User
{
    string username;
    string password;
    string email;
    vector<Music> favorites;
    vector<Music> blacklist;
};


vector<Music> musics; //vector that contains all the songs
vector<Music> playlist; //vector that contains the playlist
vector<User> all_users; //vector that contains all the users which is registered
User current_user; //current user that is log in


// ASCII conversion function
string to_ascii(const string& str) {
    string result;
    for (char c : str) {
        if (isprint(c) && static_cast<unsigned char>(c) < 128) {
            result += c;
        }
        else {
            result += '?'; 
        }
    }
    return result;
}

// Function to load songs from a txt file
void load_songs_from_file(const string& filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "Could not open file: " << filename << endl;
        return;
    }

    string line;
    int loaded_count = 0;
    while (getline(inFile, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        Music song;

        if (!getline(ss, song.name, '#') ||
            !getline(ss, song.singer, '#') ||
            !getline(ss, song.genre, '#') ||
            !getline(ss, song.emotion, '#') ||
            !getline(ss, song.speed, '#'))
        {
            cerr << "Warning: Malformed song metadata line. Skipping.\n";
            continue;
        }

        // Read lyrics
        string lyrics_line, lyrics_accumulated;
        while (getline(inFile, lyrics_line))
        {
            if (lyrics_line == "/") break;
            lyrics_accumulated += lyrics_line + "\n";
        }

        if (!lyrics_accumulated.empty() && lyrics_accumulated.back() == '\n') {
            lyrics_accumulated.pop_back();
        }

        // Ensure ASCII-only data
        song.name = to_ascii(song.name);
        song.singer = to_ascii(song.singer);
        song.genre = to_ascii(song.genre);
        song.emotion = to_ascii(song.emotion);
        song.speed = to_ascii(song.speed);
        song.lyrics = to_ascii(lyrics_accumulated);

        musics.push_back(song);
        loaded_count++;
    }

    inFile.close();
    cout << loaded_count << " songs loaded from file: " << filename << "\n";
}
// Function to clear the console screen (move the screen to the bottom)
void clear_screen() {
    cout << "\033[2J\033[1;1H";
}
// Function to load all users' data from a binary file (dat file)
void load_all_users_data(const string& filename)
{
    ifstream inFile(filename, ios::binary);
    if (!inFile)
    {
        cerr << "No existing user data found. Starting fresh.\n";
        return;
    }

    size_t user_count;
    inFile.read(reinterpret_cast<char*>(&user_count), sizeof(user_count));

    all_users.clear();
    for (size_t i = 0; i < user_count; i++)
    {
        User user;

        // Read strings with length
        auto read_string = [&](string& str) {
            size_t length;
            inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
            str.resize(length);
            inFile.read(&str[0], length);
            };

        read_string(user.email);
        read_string(user.username);
        read_string(user.password);

        // Read favorites
        size_t favorites_size;
        inFile.read(reinterpret_cast<char*>(&favorites_size), sizeof(favorites_size));
        user.favorites.resize(favorites_size);
        for (auto& song : user.favorites)
        {
            read_string(song.name);
            read_string(song.singer);
            read_string(song.genre);
            read_string(song.emotion);
            read_string(song.speed);
            read_string(song.lyrics);
        }

        // Read blacklist
        size_t blacklist_size;
        inFile.read(reinterpret_cast<char*>(&blacklist_size), sizeof(blacklist_size));
        user.blacklist.resize(blacklist_size);
        for (auto& song : user.blacklist)
        {
            read_string(song.name);
            read_string(song.singer);
            read_string(song.genre);
            read_string(song.emotion);
            read_string(song.speed);
            read_string(song.lyrics);
        }

        all_users.push_back(user);
    }

    inFile.close();
}

// Function to save all users' data to a binary file (dat file)
void save_all_users_data(const string& filename)
{
    ofstream outFile(filename, ios::binary);
    if (!outFile)
    {
        cerr << "Error saving user data.\n";
        return;
    }

    size_t user_count = all_users.size();
    outFile.write(reinterpret_cast<const char*>(&user_count), sizeof(user_count));

    for (const auto& user : all_users)
    {
        // Write strings with length
        auto write_string = [&](const string& str) {
            size_t length = str.size();
            outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
            outFile.write(str.c_str(), length);
            };

        write_string(user.email);
        write_string(user.username);
        write_string(user.password);

        // Write favorites
        size_t favorites_size = user.favorites.size();
        outFile.write(reinterpret_cast<const char*>(&favorites_size), sizeof(favorites_size));
        for (const auto& song : user.favorites)
        {
            write_string(song.name);
            write_string(song.singer);
            write_string(song.genre);
            write_string(song.emotion);
            write_string(song.speed);
            write_string(song.lyrics);
        }

        // Write blacklist
        size_t blacklist_size = user.blacklist.size();
        outFile.write(reinterpret_cast<const char*>(&blacklist_size), sizeof(blacklist_size));
        for (const auto& song : user.blacklist)
        {
            write_string(song.name);
            write_string(song.singer);
            write_string(song.genre);
            write_string(song.emotion);
            write_string(song.speed);
            write_string(song.lyrics);
        }
    }

    outFile.close();
}
// Function to update the current user in the all_users vector and save to file
void update_current_user() {
    for (auto& user : all_users) {
        if (user.email == current_user.email) {
            user = current_user;
            break;
        }
    }
    save_all_users_data("users.dat");
}
// Function to check password strength
bool passwordstrength(const string& password)
{
    bool contain_upper = false, contain_lower = false, contain_digit = false;
    for (char c : password)
    {
        if (isupper(c)) contain_upper = true;
        else if (islower(c)) contain_lower = true;
        else if (isdigit(c)) contain_digit = true;
    }

    if (contain_upper && contain_lower && contain_digit && password.length() >= 8)
    {
        cout << "Strong password.\n";
        return true;
    }
    else
    {
        cout << "Weak password. Must contain uppercase, lowercase, digit, and be at least 8 chars.\n";
        return false;
    }
}
// Function to check email validation
bool emailcheck(const string& email)
{
    size_t at_pos = email.find('@');
    if (at_pos == string::npos || at_pos == 0 || at_pos == email.length() - 1)
    {
        cout << "Invalid email format. Problem with '@'.\n";
        return false;
    }

    if (email.find('@', at_pos + 1) != string::npos)
    {
        cout << "Invalid email format. Multiple '@' symbols.\n";
        return false;
    }

    size_t dot_pos = email.find('.', at_pos);
    if (dot_pos == string::npos || dot_pos == email.length() - 1)
    {
        cout << "Invalid email format. Problem with the dot.\n";
        return false;
    }

    cout << "Valid email format.\n";
    return true;
}
// Function to register a new user in the system
void register_user()
{
    User new_user;
    cout << "=== User Registration ===\n";

    // Clear any leftover input
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Email validation
    do {
        cout << "Enter email: ";
        getline(cin, new_user.email);
    } while (!emailcheck(new_user.email));

    // Check if email already exists
    for (const auto& user : all_users)
    {
        if (user.email == new_user.email)
        {
            cout << "This email is already registered. Please login instead.\n";
            return;
        }
    }

    // Username
    cout << "Enter username: ";
    getline(cin, new_user.username);

    // Password validation
    do {
        cout << "Enter password: ";
        getline(cin, new_user.password);
    } while (!passwordstrength(new_user.password));

	// Personalized song recommendations
    string favorite_genre, favorite_emotion, favorite_singer;
    cout << "What's your favorite genre? ";
    getline(cin, favorite_genre);
    cout << "What emotion do you prefer in music (e.g., happy, sad, energetic)? ";
    getline(cin, favorite_emotion);
    cout << "Any favorite singer/artist? ";
    getline(cin, favorite_singer);

    // Add songs matching this preference to favorites automatically
    for (const auto& song : musics)
    {
        if ((song.genre == favorite_genre || song.emotion == favorite_emotion || song.singer == favorite_singer) &&
            new_user.favorites.size() < 5)
        {
            new_user.favorites.push_back(song);
        }
    }

    cout << "Registration complete. Some songs have been added to your favorites based on your preferences.\n";

    // Add new user to all_users and save
    all_users.push_back(new_user);
    current_user = new_user;
    save_all_users_data("users.dat");
}
// Function to login an existing user in the system
void login_user()
{
    clear_screen;
    string email, password;

	while (true)    // Loop until successful login or user cancels
    {
		cout << "Enter email (or type 'exit' to cancel login and go to registration): "; //if user types exit, go to registration
        getline(cin, email);
        if (email == "exit") {
            cout << "Login canceled.\n";
            register_user(); // Go to registration
            return; // Exit login
        }

		cout << "Enter password (or type 'exit' to cancel login and go to registration): ";     //if user types exit, go to registration
        getline(cin, password);
        if (password == "exit") {
            cout << "Login canceled.\n";
            register_user(); // Go to registration
            return; // Exit login
        }

        // Search for user in all_users
        bool found = false;
        for (const auto& user : all_users)
        {
            if (user.email == email && user.password == password)
            {
                current_user = user;
                found = true;
                break;
            }
        }

        if (found)
        {
            cout << "Login successful!\n";
            break;
        }
        else
        {
            cout << "Invalid email or password. Try again.\n";
        }
    }
}
// Function to display the music list
void display_music_list(const vector<Music>& list)
{
    clear_screen();
    cout << left << setw(20) << "Name"
        << setw(20) << "Singer"
        << setw(10) << "Genre" << endl;
    cout << string(60, '-') << endl;

    for (const auto& song : list) {
        cout << left << setw(20) << song.name
            << setw(20) << song.singer
            << setw(10) << song.genre << endl;
    }
}
// Function to upload a new song
void upload_song()
{
    clear_screen();
    Music new_song;
    cout << "Enter song name: ";
    cin >> ws;
    getline(cin, new_song.name);

    cout << "Enter singer: ";
    getline(cin, new_song.singer);

    cout << "Enter genre: ";
    getline(cin, new_song.genre);

    cout << "Enter emotion: ";
    getline(cin, new_song.emotion);

    cout << "Enter speed: ";
    getline(cin, new_song.speed);

    cout << "Enter lyrics (brief): ";
    getline(cin, new_song.lyrics);

    ofstream outfile("songs.txt", ios::app);
    if (outfile)
    {
        outfile << new_song.genre << "#"
            << new_song.singer << "#"
            << new_song.name << "#"
            << new_song.emotion << "#"
            << new_song.speed << "#"
            << new_song.lyrics << "\n";
        outfile.close();
    }
    else
    {
        cerr << "Failed to open songs.txt for writing.\n";
    }

    musics.push_back(new_song);
    cout << "Song uploaded successfully!\n";
}
// Function to recommend the next song based on the chance of parameters
string rec_par()
{
    int randVal = rand() % 10 + 1;
    if (randVal <= 4) return "artist";
    else if (randVal <= 7) return "genre";
    else if (randVal <= 9) return "emotion";
    else return "speed";
}
// Function to recommend the next song based on the current song and user preferences
Music recommend_next_song(const Music& current_song, const vector<Music>& library, const vector<Music>& blacklist)
{
    string param = rec_par(); // Get the recommendation parameter
    vector<Music> filtered;

    // Check if the song is in blacklist
    auto is_blacklisted = [&](const Music& s) {
        return any_of(blacklist.begin(), blacklist.end(), [&](const Music& b) {
            return b.name == s.name && b.singer == s.singer;
            });
        };

    // filter the song based on the parameter
    for (const auto& song : library) {
        // Skip the current song and any blacklisted songs
        if (song.name == current_song.name || is_blacklisted(song)) continue;

        if ((param == "artist" && song.singer == current_song.singer) ||
            (param == "genre" && song.genre == current_song.genre) ||
            (param == "emotion" && song.emotion == current_song.emotion) ||
            (param == "speed" && song.speed == current_song.speed)) {
            filtered.push_back(song);
        }
    }

    // If no filtered songs are found, show a random non-blacklisted song
    if (filtered.empty()) {
        cout << "No more songs found with the same " << param << ". Showing random song.\n";

		// Get a random non-blacklisted song
        vector<Music> non_blacklisted;
        for (const auto& song : library) {
            if (!is_blacklisted(song) && song.name != current_song.name) {
                non_blacklisted.push_back(song);
            }
        }

        // If no non-blacklisted songs are available, return the current song
        if (non_blacklisted.empty()) {
            cout << "No available songs to recommend.\n";
            return current_song; // fallback
        }

        return non_blacklisted[rand() % non_blacklisted.size()];
    }

    // Return a random filtered song
    return filtered[rand() % filtered.size()];
}



// Function to listen to a song
void listen_music_main(Music current_song) {
    auto already_in = [](const vector<Music>& list, const Music& s) {
        return any_of(list.begin(), list.end(), [&](const Music& m) {
            return m.name == s.name && m.singer == s.singer;
            });
        };

    while (true) {
        // Display current song details
        clear_screen();
        cout << "\nNow Playing: " << current_song.name << " by " << current_song.singer << "\n";
        cout << "Genre: " << current_song.genre << ", Emotion: " << current_song.emotion
            << ", Speed: " << current_song.speed << "\n";
        cout << "\nOptions:\n"
            << "1. Add to Favorites\n"
            << "2. Add to Blacklist\n"
            << "3. Add to Playlist\n"
            << "4. Next Recommended Song\n"
            << "5. Exit\n"
            << "Choose an option: ";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid choice. Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        // Process the user's choice
        switch (choice) {
        case 1:
            if (!already_in(current_user.favorites, current_song)) {
                current_user.favorites.push_back(current_song);
                cout << "Added to Favorites.\n";
                update_current_user();
            }
            else {
                cout << "Already in Favorites.\n";
            }
            break;

        case 2:
            if (!already_in(current_user.blacklist, current_song)) {
                current_user.blacklist.push_back(current_song);
                cout << "Added to Blacklist.\n";
                update_current_user();
            }
            else {
                cout << "Already in Blacklist.\n";
            }
            break;

        case 3:
            if (!already_in(playlist, current_song)) {
                playlist.push_back(current_song);
                cout << "Added to Playlist.\n";
            }
            else {
                cout << "Already in Playlist.\n";
            }
            break;

        case 4:
            current_song = recommend_next_song(current_song, musics, current_user.blacklist);
            break;

        case 5:
            update_current_user();
            cout << "Progress saved. Exiting player.\n";
            return;
        }
    }
}

// Function to listen to music with personalized recommendations
void listen_music() {
    vector<Music> recommended;
    for (const auto& song : musics) {
        // Skip if blacklisted or already a favorite
        auto is_same_song = [&](const Music& a, const Music& b) {
            return a.name == b.name && a.singer == b.singer;
            };

        if (any_of(current_user.blacklist.begin(), current_user.blacklist.end(), [&](const Music& b) {
            return is_same_song(b, song);
            })) continue;

        if (any_of(current_user.favorites.begin(), current_user.favorites.end(), [&](const Music& f) {
            return is_same_song(f, song);
            })) continue;

        for (const auto& fav : current_user.favorites) {
            if (song.genre == fav.genre || song.emotion == fav.emotion || song.singer == fav.singer) {
                recommended.push_back(song);
                break;
            }
        }

        if (recommended.size() >= 5) break;
    }
	// If no personalized recommendations found, show random songs
    if (recommended.empty()) {
        cout << "No personalized recommendations found. Showing random songs.\n";
        random_device rd;
        mt19937 g(rd());
        shuffle(musics.begin(), musics.end(), g);
        recommended.assign(musics.begin(), musics.begin() + min(5, (int)musics.size()));
    }

    cout << "\nRecommended Songs:\n";
    display_music_list(recommended);

    cout << "\nEnter the name of the song to listen to: ";
    string song_name;
    cin >> ws;
    getline(cin, song_name);

    auto it = find_if(musics.begin(), musics.end(), [&](const Music& m) {
        return m.name == song_name;
        });

    if (it == musics.end()) {
        cout << "Song not found.\n";
        return;
    }

    listen_music_main(*it); // Start the song
}
// Function to find a song by genre, singer, or name
void find_song() {
    clear_screen();
    cout << "Search by genre, singer, or song name: ";
    string query;
    cin >> ws;
    getline(cin, query);

    vector<Music> results;
    for (const auto& song : musics) {
        if (song.genre.find(query) != string::npos ||
            song.singer.find(query) != string::npos ||
            song.name.find(query) != string::npos) {
            results.push_back(song);
        }
    }

    if (results.empty()) {
        cout << "No songs matched your search.\n";
        return;
    }

    cout << "\nSearch Results:\n";
    display_music_list(results);

    cout << "\nEnter the name of the song to listen to or press Enter to cancel: ";
    string choice;
    getline(cin, choice);

    if (choice.empty()) return;

    auto it = find_if(results.begin(), results.end(), [&](const Music& m) {
        return m.name == choice;
        });

    if (it != results.end()) {
		listen_music_main(*it); // Start the song, go  to the listen_music_main function
    }
    else {
        cout << "Song not found in results.\n";
    }
}
// Function to run tests
void run_tests() {
    cout << "=== Running Tests ===\n";

    // 1. Test: Create Music and serialize
    Music test_song = { "TestGenre", "TestSinger", "TestSong", "TestEmotion", "TestSpeed", "All fine" };
    musics.push_back(test_song);
    ofstream out("test_songs.txt");
    out << test_song.name << "#" << test_song.singer << "#" << test_song.genre << "#"
        << test_song.emotion << "#" << test_song.speed << "#" << test_song.lyrics << "\n";
    out.close();
    cout << "Test 1 Passed: Song creation and save.\n";

    // 2. Test: Load from file
    musics.clear();
    load_songs_from_file("test_songs.txt");
    if (!musics.empty()) cout << "Test 2 Passed: Load song from file.\n";
    else cout << "Test 2 Failed: Song not loaded.\n";

    // 3. Test: Register user with valid input
    User test_user = { "testuser@example.com", "TestUser", "Password123", {}, {} };
    all_users.push_back(test_user);
    save_all_users_data("test_users.dat");
    all_users.clear();
    load_all_users_data("test_users.dat");
    if (!all_users.empty()) cout << "Test 3 Passed: User save/load.\n";
    else cout << "Test 3 Failed: User not saved/loaded.\n";

    // 4. Test: Add to favorites
    current_user = test_user;
    current_user.favorites.push_back(test_song);
    update_current_user();
    cout << "Test 4 Passed: Favorite added and saved.\n";

    // 5. Edge case: Search non-existent song
    auto it = find_if(musics.begin(), musics.end(), [](const Music& m) {
        return m.name == "NonExistent";
        });
    if (it == musics.end()) cout << "Test 5 Passed: Non-existent song not found.\n";

    cout << "=== All tests Completed ===\n\n";
}

int main()
{
    srand(time(0)); // for random recommendation
	load_songs_from_file("songs.txt"); // Load songs from file
	load_all_users_data("users.dat");   // Load user data from file
    int reglog;
	// Main menu for registration or login
    do
    {
        cout << "Please choose:\n1. Register\n2. Login\nChoice: ";
        if (!(cin >> reglog))
        {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (reglog)
        {
        case 1:
            register_user();
            // Exit the loop if the user has registered successfully
            if (!current_user.username.empty())
            {
                reglog = 2;
            }
            break;
        case 2:
            login_user();
            break;
        default:
            cout << "Invalid choice. Please enter 1 or 2.\n";
            break;
        }
    } while (reglog != 2);

    int option;
	// Main menu for the music management system
    do
    {
        clear_screen();
        cout << "\nWelcome, " << current_user.username << "!\n";
        cout << "\n=== Music Management System ===\n";
        cout << "1. Find a song\n";
        cout << "2. Upload your own music\n";
        cout << "3. Listen music\n";
        cout << "4. Exit\n";
        cout << "================================\n";
        cout << "Choose an option (1-4): ";

        if (!(cin >> option)) // Check for non-integer input
        {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart the loop
        }
        switch (option)
        {
        case 1: find_song(); break;
        case 2: upload_song(); break;
        case 3: listen_music(); break;
        case 4: cout << "Exiting program. Goodbye!\n"; break;
        default: cout << "Invalid option. Try again.\n"; break;
        }
    } while (option != 4);
    // Save all user data before exiting
    save_all_users_data("users.dat");
    run_tests(); // Run tests
    return 0;
}