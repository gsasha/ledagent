#include "opc/effect.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdlib.h> 
#include <time.h>       /* time */
Effect::Effect(buffer_pixel_t *pixels, int num_pixels)
    : pixels_(pixels), num_pixels_(num_pixels) {}

BlackEffect::BlackEffect(buffer_pixel_t *pixels, int num_pixels)
    : Effect(pixels, num_pixels) {}

void BlackEffect::RenderFrame() { memset(pixels_, 0, num_pixels_ * 4); }

WhiteEffect::WhiteEffect(buffer_pixel_t *pixels, int num_pixels)
    : Effect(pixels, num_pixels) {}

void WhiteEffect::RenderFrame() { memset(pixels_, 0xff, num_pixels_ * 4); }

ColorDiagnosticEffect::ColorDiagnosticEffect(buffer_pixel_t *pixels,
                                             int num_pixels)
    : Effect(pixels, num_pixels) {}

void ColorDiagnosticEffect::RenderFrame() {
  memset(pixels_, 0, num_pixels_ * 4);
  // x red, 1 black, x green, 1 white, x blue, 1 black, x w.
  int pos = 0;
  int x = 5;
  for (int i = 0; i < x; i++, pos++) {
    pixels_[pos].r = 0xff;
  }
  pixels_[pos].r = 0x0;
  pixels_[pos].g = 0x0;
  pixels_[pos].b = 0x0;
  pixels_[pos].w = 0x0;
  pos++;

  for (int i = 0; i < x; i++, pos++) {
    pixels_[pos].g = 0xff;
  }
  pixels_[pos].r = 0x0;
  pixels_[pos].g = 0x0;
  pixels_[pos].b = 0x0;
  pixels_[pos].w = 0x0;
  pos++;

  for (int i = 0; i < x; i++, pos++) {
    pixels_[pos].b = 0xff;
  }
  pixels_[pos].r = 0x0;
  pixels_[pos].g = 0x0;
  pixels_[pos].b = 0x0;
  pixels_[pos].w = 0x0;
  pos++;

  for (int i = 0; i < x; i++, pos++) {
    pixels_[pos].w = 0xff;
  }
  pixels_[pos].r = 0xff;
  pixels_[pos].g = 0xff;
  pixels_[pos].b = 0xff;
  pixels_[pos].w = 0xff;
  pos++;
}

BreatheEffect::BreatheEffect(buffer_pixel_t *pixels, int num_pixels)
    : Effect(pixels, num_pixels) {}

void BreatheEffect::RenderFrame() {
  memset(pixels_, luminance_, num_pixels_ * sizeof(buffer_pixel_t));
  if (up_) {
    luminance_ += 1;
    if (luminance_ == 250) {
      up_ = false;
    }
  } else {
    luminance_ -= 1;
    if (luminance_ == 0) {
      up_ = true;
    }
  }
}

WalkEffect::WalkEffect(buffer_pixel_t *pixels, int num_pixels, int offset)
    : Effect(pixels, num_pixels), offset_(offset), position_(offset) {}

void WalkEffect::RenderFrame() {
  memset(pixels_, 0, num_pixels_ * sizeof(buffer_pixel_t));
  *(uint32_t *)&pixels_[position_] = 0xffffffff;
  position_ += 77 + offset_;
  position_ %= num_pixels_;
}

SpikeEffect::SpikeEffect(buffer_pixel_t *pixels, int num_pixels)
    : Effect(pixels, num_pixels), offset_(0), position_(0) {
srand (time(NULL));
 memset(pixels_, 0, num_pixels_ * sizeof(buffer_pixel_t));
}

void SpikeEffect::RenderFrame() {
  
  
  int rowoffset[8] = {-1,0,1,-1,1,-1,0,1};
  int columnoffset[8] = {1,1,1,0,0,-1,-1,-1};
  int major = 14;
  int minor = 2;
  uint8_t whiteness = 0xff;
  // if((frame_id+1)%3 == 0)
    {
  for(int i=0;i<15;++i)
  for(int j=0;j<15;++j)
    {
      int summ = major;
      int poi = i*15 + j;
      uint32_t r = major*pixels_[poi].r;
      uint32_t g = major*pixels_[poi].g;
      uint32_t b = major*pixels_[poi].b;
      //uint32_t w = maj*pixels_[poi].w;
      if(r==0&&g==0&&b==0)
	{
	  summ = 0;
	}
      for(int k=0;k<8;++k)
	{
	  int id = poi+rowoffset[k]+15*columnoffset[k];
	  if ( id > 0 && id < num_pixels_ && i + columnoffset[k] >= 0 && i + columnoffset[k] < 15 && j + rowoffset[k] >= 0 && j + rowoffset[k] < 15 && pixels_[id].w != 0 )
	    {
	      r+=minor*pixels_[id].r;
	      //w+=minor*pixels_[id].w;
	      g+=minor*pixels_[id].g;
	      b+=minor*pixels_[id].b;
	      summ+=minor;
	    }
	}
      if(summ > 0)
	{
      pixels_[poi].r = (uint8_t)(r/summ);
      pixels_[poi].g = (uint8_t)(g/summ);
      pixels_[poi].b = (uint8_t)(b/summ);
      pixels_[poi].w = whiteness;
	}
    }
  if(frame_id%20 == 0)
    {
      //spike_id = (spike_id+1)%num_spikes;
  spikes[0] = rand();
    }
  spike_pos -= 11*(num_spikes-1);
  if(spike_pos < 0)
    {
      spike_pos += num_pixels_;
    }
  for(int w =0;w<11;++w)
    {
      *(uint32_t *)&pixels_[spike_pos] = spikes[0];
      pixels_[spike_pos].w = whiteness;
  spike_pos = (spike_pos +11)%num_pixels_;
    }
  }
  frame_id = (frame_id +1)%12000;
}

void HSV_to_RGB(float h, float s, float v, float *r, float *g, float *b) {
  float c = v * s;
  float x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
  float m = v - c;
  if (h >= 0.0 && h < 60.0) {
    *r = c + m;
    *g = x + m;
    *b = m;
  } else if (h >= 60.0 && h < 120.0) {
    *r = x + m;
    *g = c + m;
    *b = m;
  } else if (h >= 120.0 && h < 180.0) {
    *r = m;
    *g = c + m;
    *b = x + m;
  } else if (h >= 180.0 && h < 240.0) {
    *r = m;
    *g = x + m;
    *b = c + m;
  } else if (h >= 240.0 && h < 300.0) {
    *r = x + m;
    *g = m;
    *b = c + m;
  } else if (h >= 300.0 && h < 360.0) {
    *r = c + m;
    *g = m;
    *b = x + m;
  } else {
    *r = m;
    *g = m;
    *b = m;
  }
}

ColorFadeEffect::ColorFadeEffect(buffer_pixel_t *pixels, int num_pixels,
                                 float offset, float delta)
    : Effect(pixels, num_pixels), delta_(delta), H_(offset) {}

void ColorFadeEffect::RenderFrame() {
  float r, g, b;
  HSV_to_RGB(H_, S_, V_, &r, &g, &b);
  H_ += delta_;
  if (H_ >= 360) {
    H_ = 0;
  }

  buffer_pixel_t color;
  color.r = r * 250;
  color.g = g * 250;
  color.b = b * 250;
  uint32_t color_int = *(uint32_t *)&color;
  for (int i = 0; i < num_pixels_; i++) {
    *(uint32_t *)&pixels_[i] = color_int;
  }
}

MatrixEffect::MatrixEffect(buffer_pixel_t *pixels, int num_pixels,
                           int num_drops, bool forward)
    : Effect(pixels, num_pixels), num_drops_(num_drops), forward_(forward),
      drops_(num_drops_, 0.0), speeds_(num_drops_, 0.0),
      trail_lengths_(num_drops_, 0.0) {
  for (int i = 0; i < num_drops_; i++) {
    CreateDrop(i);
  }
}

void MatrixEffect::RenderFrame() {
  UpdateDrops();
  RenderDrops();
}

void MatrixEffect::CreateDrop(int i) {
  if (forward_) {
    drops_[i] = (rand() % num_pixels_) / 4.0;
  } else {
    drops_[i] = num_pixels_ - (rand() % num_pixels_) / 4.0;
  }
  speeds_[i] = exp(-(rand() % 100 / 50.0));
  trail_lengths_[i] = (rand() % num_pixels_) / 10. + 1;
}

void MatrixEffect::UpdateDrops() {
  if (forward_) {
    for (int i = 0; i < num_drops_; ++i) {
      drops_[i] += speeds_[i];
      if (drops_[i] - trail_lengths_[i] > num_pixels_) {
        CreateDrop(i);
      }
    }
  } else {
    for (int i = 0; i < num_drops_; ++i) {
      drops_[i] -= speeds_[i];
      if (drops_[i] + trail_lengths_[i] < 0) {
        CreateDrop(i);
      }
    }
  }
}

void MatrixEffect::RenderDrops() {
  memset(pixels_, 0, num_pixels_ * sizeof(buffer_pixel_t));
  if (forward_) {
    for (int i = 0; i < num_drops_; i++) {
      buffer_pixel_t color = color_;
      buffer_pixel_t color_diff = color_;
      color_diff.r /= trail_lengths_[i];
      color_diff.g /= trail_lengths_[i];
      color_diff.b /= trail_lengths_[i];
      int stop = drops_[i] - trail_lengths_[i];
      for (int p = drops_[i]; p > stop; p--) {
        color.r -= color_diff.r;
        color.g -= color_diff.g;
        color.b -= color_diff.b;
        if (p >= 0 && p < num_pixels_) {
          pixels_[p] = color;
        }
      }
    }
  } else {
    for (int i = 0; i < num_drops_; i++) {
      buffer_pixel_t color = color_;
      buffer_pixel_t color_diff = color_;
      color_diff.r /= trail_lengths_[i];
      color_diff.g /= trail_lengths_[i];
      color_diff.b /= trail_lengths_[i];
      int stop = drops_[i] + trail_lengths_[i];
      for (int p = drops_[i]; p < stop; p++) {
        color.r -= color_diff.r;
        color.g -= color_diff.g;
        color.b -= color_diff.b;
        if (p >= 0 && p < num_pixels_) {
          pixels_[p] = color;
        }
      }
    }
  }
}

StarsEffect::StarsEffect(buffer_pixel_t *pixels, int num_pixels, int num_stars)
    : Effect(pixels, num_pixels), num_stars_(num_stars), stars_(num_stars_),
      luminocities_(num_stars_), luminocity_limits_(num_stars),
      speeds_(num_stars_), fading_(num_stars_) {
  for (int i = 0; i < num_stars_; i++) {
    CreateStar(i);
  }
}

void StarsEffect::CreateStar(int i) {
  stars_[i] = rand() % num_pixels_;
  speeds_[i] = exp(3.0 - (rand() % 100 / 10.0));
  luminocity_limits_[i] = speeds_[i] / exp(3.0) * (rand() % 50 + 200);
  luminocities_[i] = rand() % ((int)(luminocity_limits_[i] / 4 + 1));
  fading_[i] = false;
}

void StarsEffect::RenderFrame() {
  for (int i = 0; i < num_stars_; i++) {
    if (fading_[i]) {
      luminocities_[i] -= speeds_[i];
      if (luminocities_[i] <= 0) {
        CreateStar(i);
      }
    } else {
      luminocities_[i] += speeds_[i];
      if (luminocities_[i] >= luminocity_limits_[i]) {
        fading_[i] = true;
      }
    }
  }

  memset(pixels_, 0, num_pixels_ * sizeof(buffer_pixel_t));
  for (int i = 0; i < num_stars_; i++) {
    buffer_pixel_t color;
    color.r = color.g = color.b = luminocities_[i];
    pixels_[stars_[i]] = color;
  }
}
