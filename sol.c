#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>

ma_result result;
ma_engine engine;
ma_sound sound;

#define DIRECTORY "songs/"

#define MAX_SIZE 100

int INDEX = 0;
char songDirectory[MAX_SIZE][MAX_SIZE];

int currSongINDEX = 0;

void extractFromDir() {
  system("break>songNames.txt");
  system("dir songs /b >> songNames.txt");

  FILE* songList = fopen("songNames.txt", "r");

  char currSongName[MAX_SIZE];
  while (fgets(currSongName, MAX_SIZE, songList)) {

    strcpy(songDirectory[INDEX], DIRECTORY);

    int i = 0;
    while (currSongName[i] != '\n') {
      ++i;
    }
    currSongName[i] = 0;

    strcat(songDirectory[INDEX],  currSongName);

    //printf("%s\n", songDirectory[INDEX]);
    ++INDEX;
  }
}

void currentlyPlaying(char songName[MAX_SIZE]) {
    int i = 0;
    while (songName[i] != '/') {
        ++i;
    }
    ++i;
    printf("currently Playing: ");
    while (songName[i] != 0) {
        printf("%c", songName[i++]);
    }
    printf("\n");
}

char music_player_controls() {
    printf("[N] = Next | [P] = Pause | [S] = Skip | [E] = Stop\n");
    char cc;
    scanf(" %c", &cc);
    return cc;
}

void wait_unill_music_resume_command() {
    printf("[R] = Resume...\n");
    char cc;
    scanf(" %c", &cc);
}

int audio_engine_init() {
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return -1;
    }
    return 1;
}

int terminate_audio_engine() {
    ma_engine_uninit(&engine);
}

int playAudioFile(char* NAME_OF_FILE) {
    result = ma_sound_init_from_file(&engine, NAME_OF_FILE, 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        return -1;
    }

    currentlyPlaying(NAME_OF_FILE);
    ma_sound_start(&sound);
    
    while (1) {
      
      char cc = music_player_controls();
      getchar();
      if (cc == 'E') {
          break;
      }
      if (cc == 'P') {
          ma_sound_stop(&sound);
          wait_unill_music_resume_command();
          ma_sound_start(&sound);
      } else if (cc == 'N') {
        ++currSongINDEX;
        currSongINDEX %= INDEX;

        terminate_audio_engine();
        audio_engine_init();
        
        result = ma_sound_init_from_file(&engine, songDirectory[currSongINDEX], 0, NULL, NULL, &sound);        
        if (result != MA_SUCCESS) {
            return -1;
        }

        system("cls");

        currentlyPlaying(songDirectory[currSongINDEX]);
        ma_sound_start(&sound);

        continue;
      }
      
      system("cls");
    
    }
    
    terminate_audio_engine();
    return 0;
}

int main()
{
    extractFromDir();
    audio_engine_init();
    playAudioFile(songDirectory[currSongINDEX]);
    return 0;
}
