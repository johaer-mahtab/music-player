#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>
#include <windows.h>
#include <conio.h>

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

int displayFrameToChooseSong() {
    system("cls");
    int currentIndex = 0;
    while (1) {
        printf("Control Keys: [J] = Up | [L] = Down | [k] = Enter | [E] = Exit\n\n");
        for (int i = 0; i < INDEX; i++) {
            int flag = 0;
            if (i == currentIndex) {
                printf("\x1b[31m> ");
                printf("\x1b[32m");
            } else {
                printf("  ");
            }
            int j = 0;
            while (songDirectory[i][j] != '/') {
                ++j;
            }
            ++j;
            while (songDirectory[i][j] != '\0') {
                printf("%c", songDirectory[i][j++]);
            }
            // for (int j = 0; songDirectory[i][j] != '\0'; j++) {
            //     if (songDirectory[i][j] == '/') {
            //         flag = 1;
            //         continue;
            //     }
            //     if (flag) printf("%c", songDirectory[i][j]);
            // }
            printf("\x1b[0m");
            printf("\n");
        }
        char cc = getch();
        cc = tolower(cc);
        if (cc == 'j') {
            currentIndex--;
            if (currentIndex < 0) currentIndex += INDEX;
        } else if (cc == 'l') {
            currentIndex++;
            currentIndex %= INDEX;
        } else if (cc == 'k') {
            system("cls");
            return currentIndex;
        } else if (cc == 'e') {
            exit(0);
        }
        system("cls");
    }
}

void currentlyPlaying(char songName[MAX_SIZE], int flag) {
    int i = 0;
    while (songName[i] != '/') {
        ++i;
    }
    ++i;
    printf("currently Playing: ");
    printf("\x1b[32m");
    while (songName[i] != 0) {
        printf("%c", songName[i++]);
        if (flag != 0) Sleep(125);

    }
    printf("\x1b[0m");
    printf("\n");
}

char music_player_controls(int invalid_input) {
    if (!invalid_input) {
      printf("[N] = Next | [P] = Pause | [S] = Skip | [E] = Stop\n");
    }
    // char cc;
    // scanf(" %c", &cc);
    char cc = getch();
    return cc;
}

void wait_unill_music_resume_command() {
    printf("[R] = Resume...\n");
    // char cc;
    // scanf(" %c", &cc);
    while (1) {
        char cc = getch();
        cc = tolower(cc);
        if (cc != 'r') {
            printf("PRESS R to continue...\n");
        } else {
            break;
        }
    }
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

    currentlyPlaying(NAME_OF_FILE, 1);
    ma_sound_start(&sound);

    int invalid_input = 0;
    
    while (1) {
      
      char cc = music_player_controls(invalid_input);
      //getchar();
      invalid_input = 0;
      cc = tolower(cc);
      if (cc == 'e') {
          terminate_audio_engine();
          audio_engine_init();
          playAudioFile(songDirectory[displayFrameToChooseSong()]);
      } else if (cc == 'p') {
          ma_sound_stop(&sound);
          wait_unill_music_resume_command();
          system("cls");
          currentlyPlaying(NAME_OF_FILE, 0);
          ma_sound_start(&sound);
      } else if (cc == 'n') {
        ++currSongINDEX;
        currSongINDEX %= INDEX;

        terminate_audio_engine();
        audio_engine_init();
        
        result = ma_sound_init_from_file(&engine, songDirectory[currSongINDEX], 0, NULL, NULL, &sound);        
        if (result != MA_SUCCESS) {
            return -1;
        }

        system("cls");

        currentlyPlaying(songDirectory[currSongINDEX], 1);
        ma_sound_start(&sound);

        continue;
      } else {
        invalid_input = 1;
      }
      
      //if (cc == 'n') system("cls");
    
    }
    
    terminate_audio_engine();
    return 0;
}

int main()
{
    extractFromDir();
    currSongINDEX = displayFrameToChooseSong();
    audio_engine_init();
    playAudioFile(songDirectory[currSongINDEX]);
    return 0;
}
