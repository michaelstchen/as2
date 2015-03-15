#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <png.h>
#include "raytracer.h"


//****************************************************
// Helper Function to Write PNG
//****************************************************

int writeImage(char const* filename, int width, int height, ImgPlane* b) {
    	int code = 0;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width ; x++) {
                    Color* c = b->getPixelColor(x, y);
                    row[x*3] = c->bit8R();
                    row[x*3 + 1] = c->bit8B();
                    row[x*3 + 2] = c->bit8G();
                    delete c;
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}


//****************************************************
// Program Starting Point
//****************************************************
int main(int argc, char* argv[]) {

    int width = 500; int height = 500;

    Point* camera = new Point(0.0, 0.0, -5.0);
    ImgPlane* view = new ImgPlane(new Point(-5.0,-5.0,0.0), new Point(5.0,-5.0,0.0), new Point(-5.0,5.0,0.0), new Point(5.0,5.0,0.0), width, height);
    World* world = new World();

    world->addShape(new Sphere(new Point(-1.0,-1.0,1.0), 3.0, world, NULL));

    Scene* scene = new Scene(world, view, camera);

    scene->render();

    printf("Saving PNG\n");
    int result = writeImage("output.png", width, height, scene->view);
    
    world->clearMem();
    view->clearMem();
    scene->clearMem();

    return result;
}
