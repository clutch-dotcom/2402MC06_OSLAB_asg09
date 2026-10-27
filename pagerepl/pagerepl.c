#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_PAGES 30

// Reference string given in the question
int ref_str[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
int ref_len = 20;

// Print current state of frames
void print_frames(int frames[], int num_frames) {
  printf(1, "[");
  for (int i = 0; i < num_frames; i++) {
    if (frames[i] == -1)
      printf(1, " - ");
    else
      printf(1, " %d ", frames[i]);
  }
  printf(1, "]");
}

// Simulate FIFO Page Replacement Algorithm
int run_fifo(int num_frames, int verbose) {
  int frames[10];
  int faults = 0;
  int fifo_idx = 0;

  for (int i = 0; i < num_frames; i++)
    frames[i] = -1;

  if (verbose)
    printf(1, "\n--- FIFO Page Replacement (%d Frames) ---\n", num_frames);

  for (int i = 0; i < ref_len; i++) {
    int page = ref_str[i];
    int hit = 0;

    // Check if page is already in frames
    for (int j = 0; j < num_frames; j++) {
      if (frames[j] == page) {
        hit = 1;
        break;
      }
    }

    if (!hit) {
      faults++;
      frames[fifo_idx] = page;
      fifo_idx = (fifo_idx + 1) % num_frames;
    }

    if (verbose) {
      printf(1, "Ref %2d: ", page);
      print_frames(frames, num_frames);
      if (hit)
        printf(1, " -> HIT\n");
      else
        printf(1, " -> FAULT\n");
    }
  }
  return faults;
}

// Simulate LRU Page Replacement Algorithm
int run_lru(int num_frames, int verbose) {
  int frames[10];
  int last_used[10];
  int faults = 0;

  for (int i = 0; i < num_frames; i++) {
    frames[i] = -1;
    last_used[i] = -1;
  }

  if (verbose)
    printf(1, "\n--- LRU Page Replacement (%d Frames) ---\n", num_frames);

  for (int i = 0; i < ref_len; i++) {
    int page = ref_str[i];
    int hit = 0;

    // Check if page is present
    for (int j = 0; j < num_frames; j++) {
      if (frames[j] == page) {
        hit = 1;
        last_used[j] = i; // Update last used timestamp
        break;
      }
    }

    if (!hit) {
      faults++;
      int replace_idx = -1;

      // Check if there is an empty frame
      for (int j = 0; j < num_frames; j++) {
        if (frames[j] == -1) {
          replace_idx = j;
          break;
        }
      }

      // If no empty frame, find the Least Recently Used frame
      if (replace_idx == -1) {
        int min_time = last_used[0];
        replace_idx = 0;
        for (int j = 1; j < num_frames; j++) {
          if (last_used[j] < min_time) {
            min_time = last_used[j];
            replace_idx = j;
          }
        }
      }

      frames[replace_idx] = page;
      last_used[replace_idx] = i;
    }

    if (verbose) {
      printf(1, "Ref %2d: ", page);
      print_frames(frames, num_frames);
      if (hit)
        printf(1, " -> HIT\n");
      else
        printf(1, " -> FAULT\n");
    }
  }
  return faults;
}

int main(void) {
  printf(1, "====================================================\n");
  printf(1, "        PAGE REPLACEMENT SIMULATION (FIFO vs LRU)    \n");
  printf(1, "====================================================\n");

  // Detailed step-by-step trace for 3 frames
  int fifo_3 = run_fifo(3, 1);
  int lru_3 = run_lru(3, 1);

  printf(1, "\nSummary for 3 frames:\n");
  printf(1, "FIFO Page Faults: %d (Expected: 15)\n", fifo_3);
  printf(1, "LRU Page Faults:  %d (Expected: 12)\n\n", lru_3);

  // Comparison Table for 3, 4, and 5 frames
  printf(1, "====================================================\n");
  printf(1, "  Frames | Algo | Faults | Hits | Hit Ratio (%%)\n");
  printf(1, "====================================================\n");

  for (int f = 3; f <= 5; f++) {
    int ff = run_fifo(f, 0);
    int fh = ref_len - ff;
    int f_ratio = (fh * 100) / ref_len;
    printf(1, "    %d    | FIFO |   %d   |  %d  |     %d%%\n", f, ff, fh, f_ratio);

    int lf = run_lru(f, 0);
    int lh = ref_len - lf;
    int l_ratio = (lh * 100) / ref_len;
    printf(1, "    %d    | LRU  |   %d   |  %d  |     %d%%\n", f, lf, lh, l_ratio);
    printf(1, "----------------------------------------------------\n");
  }

  exit();
}
