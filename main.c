#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#define VIDEO_WIDTH 1920
#define VIDEO_HEIGHT 1440

typedef unsigned char byte;
typedef byte rgb[3];
typedef rgb frame[VIDEO_HEIGHT][VIDEO_WIDTH];

#define CHAR_WHITE 'X'
#define CHAR_BLACK '.'

// Bad apple video path
const char* video_path = "bad_apple.mp4";

// Terminal width and height
int rows, cols;

void DrawVideo(FILE* video_fp)
{
    // Size per character in terminal
    int char_size_y = VIDEO_HEIGHT / rows;
    int char_size_x = VIDEO_WIDTH / cols;

    frame current_frame;
    
    while (1)
    {
        int amount_red = fread(current_frame, VIDEO_WIDTH*VIDEO_HEIGHT*3, 1, video_fp);

        // Exit loop when no image
        if (amount_red != 1) break;

        // Clear window
        clear();

        // Draw on screen
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {
                if (current_frame[y*char_size_y][x*char_size_x][0] < 10 &&
                        current_frame[y*char_size_y][x*char_size_x][1] < 10 &&
                        current_frame[y*char_size_y][x*char_size_x][2] < 10)
                {
                    // Draw character
                    mvaddch(y, x, CHAR_BLACK);
                }
                else
                {
                    // Draw character
                    mvaddch(y, x, CHAR_WHITE);
                }
            }
        }
        
        // Refresh screen
        refresh();

        // Sleep
        usleep(1000 / 60 * 1000);
    }
}

int main(int argc, char** argv)
{
    // Init screen
    initscr();

    char ffmpeg_cmd[100];
    snprintf(ffmpeg_cmd, 100, "ffmpeg -i %s -f image2pipe -s 1920x1440 -vcodec rawvideo -pix_fmt rgb24 -", video_path);
    
    // Open pipe with ffmpeg
    FILE* pipein = popen(ffmpeg_cmd, "r");
    
    // Get size of terminal
    getmaxyx(stdscr, rows, cols);
    
    DrawVideo(pipein);
    
    endwin();

    // Close ffmpeg pipe
    pclose(pipein);
    
    return 0;
}
