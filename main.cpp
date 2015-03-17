#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <png.h>
#include "raytracer.h"
#include "light.h"
#include "property.h"

//***************************************************
// Parses input file, initializes objects, renders scene,
// writes pixel data to png buffer. 
//***************************************************



/* Helper Function to Write PNG */

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
                    row[x*3 + 1] = c->bit8G();
                    row[x*3 + 2] = c->bit8B();
                    //delete c;
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

/* GLOBAL VARIABLES */
Point* camera;

ImgPlane* view;
int width = 1000; int height = 1000;

World* world = new World();

Scene* scene;


/* Program Starting Point */
int main(int argc, char* argv[]) {

    camera = new Point(0, 0, 10);
    view = new ImgPlane(new Point(-5,-5,5), new Point(5,-5,5), new Point(-5,5,5), new Point(5,5,5), width, height);

    Material* m1 = new Material(new Color(0.1, 0, 0.1), new Color(1, 0, 1), new Color(1, 1, 1), new Color(.3, .3, .3), 32);
    Material* m2 = new Material(new Color(0, 0.1, 0.1), new Color(0, 1, 1), new Color(1, 1, 1), new Color(.3, .3, .3), 32);

    world->addShape(new Sphere(new Point(-3,0,0), 3.0, world, NULL, m1));
    world->addShape(new Sphere(new Point(3,0,0), 3.0, world, NULL, m2));

    //world->addLight(new Point_Light(new Color(1,1,1), new Point(25,0,-5), 0));
    world->addLight(new Direct_Light(new Color(1,1,1), new Vector(0,0,-1)));
    //world->addLight(new Ambient_Light(new Color(1, 1, 1)));

    scene = new Scene(world, view, camera);
    scene->render();

    printf("Saving PNG\n");
    int result = writeImage("output.png", width, height, scene->view);
    
    //world->clearMem();
    //view->clearMem();
    //scene->clearMem();
    
    return result;
}
