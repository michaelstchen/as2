#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <malloc.h>
#include <png.h>
#include "raytracer.h"
#include "light.h"
#include "property.h"

//***************************************************
// Parses input file, initializes objects, renders scene,
// writes pixel data to png buffer. 
//***************************************************

/* GLOBAL VARIABLES */
Point* camera;

ImgPlane* view;
int width = 1000; int height = 1000;

World* world = new World();

Scene* scene;

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

/* Helper Function to Parse OBJ Files */
void readOBJ(char* filename, World* w, Matrix* t, Material* m) {
    if (filename == NULL) {
        fprintf(stderr, "ERROR: No file specified for obj command\n");
        return;
    }

    FILE* finput = fopen(filename, "r");
    if (finput == NULL) {
        fprintf(stderr, "ERROR: cannot open obj file '%s'\n", filename);
        return;
    }

    char line[100];
    char* pch;
    vector<Point*> pt;
    vector<Vector*> vec;
    while(fgets(line, sizeof(line), finput)) {
        if ((pch = strtok(line, " \t \n")) == NULL) continue;

        vector<float> p;
        char* params;

        while ((params = strtok(NULL, "\t\n /")) != NULL) {
            p.push_back(atof(params));
        }

        if (!strcmp(pch, "v")) {
            pt.push_back(new Point(p[0],p[1],p[2]));
        } else if (!strcmp(pch, "vn")) {
            vec.push_back(new Vector(p[0],p[1],p[2]));
        } else if (!strcmp(pch, "f") && p.size() == 3) {
            world->addShape(new Triangle(pt[(int)p[0]-1],
                                         pt[(int)p[1]-1],
                                         pt[(int)p[2]-1], w, t->copy(), m));
        } else if (!strcmp(pch, "f") && p.size() == 6) {
            world->addShape(new Triangle(pt[(int)p[0]-1],
                                         pt[(int)p[2]-1],
                                         pt[(int)p[4]-1],
                                         vec[(int)p[1]-1],
                                         vec[(int)p[3]-1],
                                         vec[(int)p[5]-1],
                                         w, t->copy(), m));
        }

    }

}

/* Helper Function to Parse Instructions From File */
int readFile(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: Please specify one input file\n");
        return 1;
    }

    FILE* finput = fopen(argv[1], "r");
    if (finput == NULL) {
        fprintf(stderr, "ERROR: cannot open file '%s'\n", argv[1]);
        return 2;
    }

    Matrix* t = new Matrix();
    Material* mat;

    char line[110];
    char* pch;
    while(fgets(line, sizeof(line), finput)) {
        if ((pch = strtok(line, " \t \n")) == NULL) continue;
        vector<float> p;
        bool arg_err = false;

        char* params;

	if (!strcmp(pch, "obj")) {
	  params = strtok(NULL, "\t \n");
	  readOBJ(params, world, t, mat);
	  continue;
	}

        while ((params = strtok(NULL, " \t\n")) != NULL) {
            p.push_back(atof(params));
        }

        if (!strcmp(pch, "cam")) {
            if (p.size() == 15) {
                camera = new Point(p[0], p[1], p[2]);
                view = new ImgPlane(new Point(p[3],p[4],p[5]), 
                                    new Point(p[6],p[7],p[8]), 
                                    new Point(p[9],p[10],p[11]), 
                                    new Point(p[12],p[13],p[14]), 
                                    width, height);
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "sph")) {
            if (p.size() == 4) {
                world->addShape(new Sphere(new Point(p[0],p[1],p[2]), p[3],
                                           world, t->copy(), mat));
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "tri")) {
	  if (p.size() == 9) {
	    world->addShape(new Triangle(new Point(p[0],p[1],p[2]),
					 new Point(p[3],p[4],p[5]),
					 new Point(p[6],p[7],p[8]),
                                         world, t->copy(), mat));
	  } else {
	    arg_err = true;
	  }
        } else if (!strcmp(pch, "ltp")) {
            if (p.size() == 6) {
                world->addLight(new Point_Light(new Color(p[3],p[4],p[5]),
                                                new Point(p[0],p[1],p[2]),
                                                0));
            } else if (p.size() == 7) {
                world->addLight(new Point_Light(new Color(p[3],p[4],p[5]),
                                                new Point(p[0],p[1],p[2]),
                                                p[6]));
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "ltd")) {
            if (p.size() == 6) {
                world->addLight(new Direct_Light(new Color(p[3],p[4],p[5]),
                                                 new Vector(p[0],p[1],p[2])));
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "lta")) {
            if (p.size() == 3) {
                world->addLight(new Ambient_Light(new Color(p[0],p[1],p[2])));
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "mat")) {
            if (p.size() == 13) {
                mat = new Material(new Color(p[0],p[1],p[2]),
                                   new Color(p[3],p[4],p[5]),
                                   new Color(p[6],p[7],p[8]),
                                   new Color(p[10],p[11],p[12]), p[9]);
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "xft")) {
            if (p.size() == 3) {
                Matrix* transl = makeTransl(p[0], p[1], p[2]);
                Matrix* oldT = t;
                t = compose(oldT, transl);
                delete transl; delete oldT;
            } else {
                arg_err = true;
            }            
        } else if (!strcmp(pch, "xfr")) {
            if (p.size() == 3) {
                Matrix* rot = makeRot(p[0], p[1], p[2]);
                Matrix* oldT = t;
                t = compose(oldT, rot);
                delete rot; delete oldT;
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "xfs")) {
            if (p.size() == 3) {
                Matrix* scale = makeScale(p[0], p[1], p[2]);
                Matrix* oldT = t;
                t = compose(oldT, scale);
                delete scale; delete oldT;
            } else {
                arg_err = true;
            }
        } else if (!strcmp(pch, "xfz")) {
            delete t;
            t = new Matrix();
        } else {
            fprintf(stderr, "ERROR: unrecognized command '%s'\n", pch);
        }

        if (arg_err) fprintf(stderr, "ERROR: parameter error for '%s'\n", pch);
    }
    return 0;
}


/* Program Starting Point */
int main(int argc, char* argv[]) {

    printf("Reading File\n");
    int read_ret = readFile(argc, argv);
    if (read_ret) return read_ret;

    printf("Rendering Scene\n");
    scene = new Scene(world, view, camera);
    scene->render();

    printf("Saving PNG\n");
    int result = writeImage("output.png", width, height, scene->view);
    
    return result;
}
