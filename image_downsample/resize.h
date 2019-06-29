// resize.h
#ifndef RESIZE_H
#define RESIZE_H

// Represent scale as fraction SCALE_GRAN / ratio
#define SCALE_GRAN 256

typedef ihc::stream_in<unsigned int> input_image_stream;
typedef ihc::stream_out<unsigned int> output_image_stream;

component void resize(unsigned ratio, int rows, int cols, input_image_stream& original_image, output_image_stream& resized_image);

#endif // RESIZE_H
