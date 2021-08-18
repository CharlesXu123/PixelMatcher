/**
 * These are some basic tests for your Pixel Marcher implementation. These are 
 * bycno means comprehensive, and you *MUST* be testing this with additional 
 * testccases of your own to ensure correctness.
 * 
 * NOTE: This file does NOT test the paths output by your findPath() program,
 *        just the final lowest cost. The automarker however WILL check if
 *        the path is correct (and includes both start / end pixels). You
 *        must make sure these are correct yourself.
 * 
 * Feel free to modify this file / add tests, etc. You will not be submitting
 * this.
 * 
 * Compile with:
 *     `make test_marcher`
 *             OR
 *     `gcc test_marcher.c imgutils.c marcher.c minheap.c -o test_marcher -lm`
 * 
 * and run with:
 *    `./test_marcher`   (or `.\test_marcher.exe` if on Windows)
 * 
 * CSCB63 Winter 2021 - Assignment 2/3
 * (c) Mustafa Quraish
 */

#include "marcher.h"
#include "unittest.h"

/****************************** Weight Functions *****************************/

// Weight function (1)
//  The weight between two pixels is the euclidean distance between
//  the (R,G,B) values of the 2 pixels. (If we think of them as vectors)
double similarColour(Image *im, int a, int b) {
  Pixel col1 = getPixel(im, a), col2 = getPixel(im, b);
  double a1 = (col1.R - col2.R);
  double b1 = (col1.G - col2.G);
  double c1 = (col1.B - col2.B);
  double sumSq = (a1 * a1) + (b1 * b1) + (c1 * c1);
  return sqrt(sumSq) + 0.01;
}

// Weight function (2)
//   The weight between two pixels is simply how close the 2nd pixel is 
//   from the colour white (255, 255, 255).
double howWhite(Image *im, int a, int b) {
  // Don't even use the current pixel's colur here... the weight
  // functions are arbitrary :)
  Pixel col2 = getPixel(im, b);
  double a1 = (255 - col2.R);
  double b1 = (255 - col2.G);
  double c1 = (255 - col2.B);
  double sumSq = (a1 * a1) + (b1 * b1) + (c1 * c1);
  return sqrt(sumSq / 100.0) + 0.01;
}

/*****************************************************************************/


void run_test(char *filename, WeightFunc wf, double expectedCost) {
  Image *img = readPPMimage(filename);          
  int *path = calloc(sizeof(int), img->sx * img->sy);
  double cost = findPath(img, wf, path);              
  outputPath(path, img);
  if (fabs(cost - expectedCost) >= 10e-4)
    TEST_FAIL("Cost (%f) did not match expected answer (%f).\n",
              cost, expectedCost);
  free(path);
  freeImage(img);
}

TEST(water)   { run_test("images/water.ppm"  , similarColour, 1280.81526); }
TEST(spiral)  { run_test("images/spiral.ppm" , similarColour, 991.255407); }
TEST(maze)    { run_test("images/maze.ppm"   , howWhite     ,  12.400000); }
TEST(bigmaze) { run_test("images/bigmaze.ppm", howWhite     ,   8.620000); }
TEST(grad)    { run_test("images/grad.ppm"   , similarColour, 278.751493); }

TEST(all_colour_weight) {
  Image *img = readPPMimage("images/25colours.ppm");          
  int *path = calloc(sizeof(int), img->sx * img->sy);
  double cost = findPath(img, allColourWeight, path);   
  printf("cost = %f\n", cost);
  int i = 0;
  while(path[i] != -1){
    printf("%d ", path[i]);
    i++;
  }
  printf("(Outputting image: inspect this manually) ");
  outputPath(path, img);
  free(path);
  freeImage(img);
}

int main(int argc, char *argv[]) {
  unit_main(argc, argv);

  printf("\n"
  " - You have passed the test cases, your solution is on the right track.\n" 
  " - It is left up to you to verify that your path itself is correct, and\n"
  " - that allColourWeight() works as intended...\n"
  );

  return 0;
}