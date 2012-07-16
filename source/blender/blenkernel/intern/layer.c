/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * Contributor(s): Blender Foundation, 2006, full recode
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/blenkernel/intern/layer.c
 *  \ingroup bke
 */
 
#include <stdio.h>
#include <string.h>

#include "MEM_guardedalloc.h"
#include "IMB_imbuf_types.h"
#include "IMB_imbuf.h"

#include "DNA_userdef_types.h"

#include "BLI_blenlib.h"
#include "BLI_utildefines.h"
#include "BLI_math_base.h"

//#include "BKE_icons.h"
//#include "BKE_global.h"
#include "BKE_image.h"
#include "BKE_layer.h"
//#include "BKE_library.h"

 
ImageLayer *layer_alloc(Image *ima, const char *name)
{
	ImageLayer *im_l;
	
	im_l = (ImageLayer*) MEM_callocN(sizeof(ImageLayer), "image_layer");
	if(im_l) {
		strcpy(im_l->name, name);
		imalayer_unique_name(im_l, ima);
		im_l->next = im_l->prev = NULL;

		im_l->background = IMA_LAYER_BG_ALPHA;
		im_l->color_space = IMA_LAYER_COL_RGB;
		im_l->opacity = 1.0f;
		im_l->mode = IMA_LAYER_NORMAL;
		im_l->type = IMA_LAYER_LAYER;
		im_l->visible = IMA_LAYER_VISIBLE;
		im_l->select = IMA_LAYER_SEL_CURRENT;
	}
	return im_l;
}

void image_free_image_layers(struct Image *ima)
{
	ImageLayer *img_lay, *next;
	ImBuf *ibuf;
	void *lock;
 
	if(ima->imlayers.first == NULL)
		return;

	ibuf= BKE_image_acquire_ibuf(ima, NULL, &lock, IMA_IBUF_IMA);
	while((img_lay=ima->imlayers.first)) {
		BLI_remlink(&ima->imlayers, img_lay);
		free_image_layer(img_lay);
	}

	ima->Count_Layers = 0;
	BKE_image_release_ibuf(ima, lock);
}
 
void free_image_layer(ImageLayer *layer)
{
	ImBuf *ibuf, *next;
 
	if (!layer)
		return;

	while((ibuf = layer->ibufs.first)) {
		BLI_remlink(&layer->ibufs, ibuf);
 
		if (ibuf->userdata) {
			MEM_freeN(ibuf->userdata);
			ibuf->userdata = NULL;
		}
 
		IMB_freeImBuf(ibuf);
	}

	MEM_freeN(layer);
}
 
ImageLayer *imalayer_get_layer_index(struct Image *ima, short value)
{
	ImageLayer *layer;
	short i;

	if(ima == NULL)
		return NULL;

	for(layer=ima->imlayers.last, i = BLI_countlist(&ima->imlayers)-1; layer; layer=layer->prev, i--)
		if(i == value)
			return layer;

	return NULL;
}

ImageLayer *imalayer_get_current(Image *ima)
{
	ImageLayer *layer;
	if(ima == NULL)
		return 0;
 
	for(layer=ima->imlayers.last; layer; layer=layer->prev){
		if(layer->select & IMA_LAYER_SEL_CURRENT)
			return layer;
	}
 
	return NULL;
}

short imalayer_get_count(Image *ima)
{
	if(ima == NULL)
		return 0;
 
	return ima->Count_Layers;
}

short imalayer_get_current_act(Image *ima)
{
	if(ima == NULL)
		return 0;
 
	return ima->Act_Layers;
}

short imalayer_get_index_layer(struct Image *ima, struct ImageLayer *iml)
{
	ImageLayer *layer;
	short i;
 
	if(ima == NULL)
		return -1;

	for(layer=ima->imlayers.last, i = BLI_countlist(&ima->imlayers)-1; layer; layer=layer->prev, i--)
		if(layer == iml)
			return i;

	return -1;
}
 
void imalayer_set_current_act(Image *ima, short index)
{
	ImageLayer *layer;
	short i;
 
	if(ima == NULL)
		return;

	for(layer=ima->imlayers.last, i = BLI_countlist(&ima->imlayers)-1; layer; layer=layer->prev, i--) {
		if(i == index) {
			layer->select = IMA_LAYER_SEL_CURRENT;
			ima->Act_Layers = i;
		}
		else
			layer->select = !IMA_LAYER_SEL_CURRENT;
	}
}

int imalayer_is_locked(struct Image *ima)
{
	ImageLayer *layer= NULL;

	layer = imalayer_get_current(ima);

	return layer->lock;
}

void imalayer_fill_color(struct Image *ima, float color[4])
{
	ImageLayer *layer= NULL;
	ImBuf *ibuf= NULL;
	unsigned char *rect= NULL;
	float *rect_float= NULL;
	void *lock;
 
	if (ima==NULL)
		return;
 
	layer = imalayer_get_current(ima);
	
	//ibuf = BKE_image_acquire_ibuf(ima, NULL, &lock);

	ibuf = (ImBuf*)((ImageLayer*)layer->ibufs.first);
	if (ibuf) {
		if (ibuf->flags & IB_rectfloat) {
			rect_float= (float*)ibuf->rect_float;
		}
		else {
			rect= (unsigned char*)ibuf->rect;
		}
 
		BKE_image_buf_fill_color(rect, rect_float, ibuf->x, ibuf->y, color);
	}
 
	//BKE_image_release_ibuf(ima, lock);
}

ImageLayer *image_duplicate_current_image_layer(Image *ima)
{
	ImageLayer *layer = NULL, *im_l = NULL;
	char dup_name[sizeof(layer->name)];
	ImBuf *ibuf, *new_ibuf;
	void *lock;
 
	if(ima==NULL)
		return NULL;
 
	layer = imalayer_get_current(ima);

	if(!strstr(layer->name, "_copy")) {
		BLI_snprintf(dup_name, sizeof(dup_name), "%s_copy", layer->name);
	}
	else {
		BLI_snprintf(dup_name, sizeof(dup_name), "%s", layer->name);
	}

	im_l = layer_alloc(ima, dup_name);
	if (im_l) {
		ibuf= (ImBuf*)((ImageLayer*)layer->ibufs.first);
		if (ibuf) {
			new_ibuf = IMB_dupImBuf(ibuf);
			BLI_addtail(&im_l->ibufs, new_ibuf);

			im_l->next = im_l->prev = NULL;
			if (layer) {
				BLI_insertlinkbefore(&ima->imlayers, layer, im_l);
			}
			else {
				BLI_addhead(&ima->imlayers, layer);
			}
			imalayer_set_current_act(ima, imalayer_get_current_act(ima));

			im_l->background = layer->background;
			im_l->color_space = layer->color_space;
			copy_v4_v4(im_l->default_color, layer->default_color);
			strcpy(im_l->file_path, layer->file_path);
			im_l->lock = layer->lock;
			im_l->mode = layer->mode;
			im_l->opacity = layer->opacity;
			im_l->type = IMA_LAYER_LAYER;
			im_l->visible = layer->visible;
		}
		ima->Count_Layers += 1;
	}
	return im_l;
}
 
int image_remove_layer(Image *ima, const int action)
{
	ImageLayer *layer= NULL;
	 
	if (ima==NULL)
		return FALSE;
 
	if (action & IMA_LAYER_DEL_SELECTED) {
		if (ima->Count_Layers > 1) {
			layer = imalayer_get_current(ima);
			if (layer) {
				BLI_remlink(&ima->imlayers, layer);
				free_image_layer(layer);
			}
			/* Ensure the first element in list gets selected (if any) */
			if (ima->imlayers.first) {
				if (imalayer_get_current_act(ima) != 1)
					imalayer_set_current_act(ima, imalayer_get_current_act(ima));
				else
					imalayer_set_current_act(ima, imalayer_get_current_act(ima)-1);
			}
			ima->Count_Layers -= 1;
		}
		else
			return -1;
	}
	else {
		if (ima->Count_Layers > 1) {
			for (layer=ima->imlayers.last; layer; layer=layer->prev){
				if (!(layer->visible & IMA_LAYER_VISIBLE)) {
					BLI_remlink(&ima->imlayers, layer);
					free_image_layer(layer);
					if (ima->imlayers.first) {
						if (imalayer_get_current_act(ima) != 1)
							imalayer_set_current_act(ima, imalayer_get_current_act(ima));
						else
							imalayer_set_current_act(ima, imalayer_get_current_act(ima)-1);
					}
					ima->Count_Layers -= 1;
				}
			}
		}
		else
			return -1;
	}
	return TRUE;
}

static char blend_normal(const char B, const char L, float O)
{	
	return (char)(O * (L) + (1.0f - O) * B);
}

static char blend_lighten(const char B, const char L, float O)
{	
	return (char)(O * ((L > B) ? L : B) + (1.0f - O) * B);
}

static char blend_darken(const char B, const char L, float O)
{	
	return (char)(O * ((L > B) ? B : L) + (1.0f - O) * B);
}

static char blend_multiply(const char B, const char L, float O)
{	
	return (char)(O * ((B * L) / 255) + (1.0f - O) * B);
}

static char blend_average(const char B, const char L, float O)
{	
	return (char)(O * ((B + L) / 2) + (1.0f - O) * B);
}

static char blend_add(const char B, const char L, float O)
{	
	return (char)(O * (MIN2(255, (B + L))) + (1.0f - O) * B);
}

static char blend_subtract(const char B, const char L, float O)
{	
	return (char)(O * ((B + L < 255) ? 0 : (B + L - 255)) + (1.0f - O) * B);
}

static char blend_difference(const char B, const char L, float O)
{	
	return (char)(O * (abs(B - L)) + (1.0f - O) * B);
}

static char blend_negation(const char B, const char L, float O)
{	
	return (char)(O * (255 - abs(255 - B - L)) + (1.0f - O) * B);
}

static char blend_screen(const char B, const char L, float O)
{	
	return (char)(O * (255 - (((255 - B) * (255 - L)) >> 8)) + (1.0f - O) * B);
}

static char blend_exclusion(const char B, const char L, float O)
{	
	return (char)(O * (B + L - 2 * B * L / 255) + (1.0f - O) * B);
}

static char blend_overlay(const char B, const char L, float O)
{	
	return (char)(O * ((L < 128) ? (2 * B * L / 255) : (255 - 2 * (255 - B) * (255 - L) / 255)) + (1.0f - O) * B);
}

static char blend_soft_light(const char B, const char L, float O)
{	
	return (char)(O * ((L < 128) ? (2 * ((B >> 1) + 64)) * ((float)L / 255) : (255 - (2 * (255 - ((B >> 1) + 64)) * (float)(255 - L) / 255))) + (1.0f - O) * B);
}

static char blend_hard_light(const char B, const char L, float O)
{	
	return (char)(O * ((B < 128) ? (2 * L * B / 255) : (255 - 2 * (255 - L) * (255 - B) / 255)) + (1.0f - O) * B);
}

static char blend_color_dodge(const char B, const char L, float O)
{	
	return (char)(O * ((L == 255) ? L : MIN2(255, ((B << 8 ) / (255 - L)))) + (1.0f - O) * B);
}

static char blend_color_burn(const char B, const char L, float O)
{	
	return (char)(O * ((L == 0) ? L : MAX2(0, (255 - ((255 - B) << 8 ) / L))) + (1.0f - O) * B);
}

static char blend_linear_dodge(const char B, const char L, float O)
{	
	return (char)(O * (MIN2(255, (B + L))) + (1.0f - O) * B);
}

static char blend_linear_burn(const char B, const char L, float O)
{	
	return (char)(O * ((B + L < 255) ? 0 : (B + L - 255)) + (1.0f - O) * B);
}

static char blend_linear_light(const char B, const char L, float O)
{	
	return (char)(O * ((2 * L) < 128) ? ((B + (2 * L) < 255) ? 0 : (B + (2 * L) - 255)) : (MIN2(255, (B + (2 * (L - 128))))) + (1.0f - O) * B);
}

static char blend_vivid_light(const char B, const char L, float O)
{	
	return (char)(O * (L < 128) ? (((2 * L) == 0) ? (2 * L) : MAX2(0, (255 - ((255 - B) << 8 ) / (2 * L)))) : (MIN2(255, (B + (2 * (L - 128))))) + (1.0f - O) * B);
}

static char blend_pin_light(const char B, const char L, float O)
{	
	return (char)(O * (L < 128) ? (((2 * L) > B) ? B : (2 * L)) : (((2 * (L - 128)) > B) ? (2 * (L - 128)) : B) + (1.0f - O) * B);
}

static char blend_hard_mix(const char B, const char L, float O)
{	
	return (char)(O * (((L < 128) ? (((2 * L) == 0) ? (2 * L) : MAX2(0, (255 - ((255 - B) << 8 ) / (2 * L)))) : (MIN2(255, (B + (2 * (L - 128))))) < 128) ? 0 : 255) + (1.0f - O) * B);
}

/*static float blend_normal_f(const float B, const float L, float O)
{	
	return (float)(O * (L) + (1.0f - O) * B);
}

static float blend_lighten_f(const float B, const float L, float O)
{	
	return (float)(O * ((L > B) ? L : B) + (1.0f - O) * B);
}

static float blend_darken_f(const float B, const float L, float O)
{	
	return (float)(O * ((L > B) ? B : L) + (1.0f - O) * B);
}

static float blend_multiply_f(const float B, const float L, float O)
{	
	return (float)(O * ((B * L) / 255) + (1.0f - O) * B);
}

static float blend_average_f(const float B, const float L, float O)
{	
	return (float)(O * ((B + L) / 2) + (1.0f - O) * B);
}

static float blend_add_f(const float B, const float L, float O)
{	
	return (float)(O * (MIN2(255, (B + L))) + (1.0f - O) * B);
}

static float blend_subtract_f(const float B, const float L, float O)
{	
	return (float)(O * ((B + L < 255) ? 0 : (B + L - 255)) + (1.0f - O) * B);
}

static float blend_difference_f(const float B, const float L, float O)
{	
	return (float)(O * (abs(B - L)) + (1.0f - O) * B);
}

static float blend_negation_f(const float B, const float L, float O)
{	
	return (float)(O * (255 - abs(255 - B - L)) + (1.0f - O) * B);
}

static float blend_screen_f(const float B, const float L, float O)
{	
	return (float)(O * (255 - (FTOCHAR((255 - B) * (255 - L)) >> 8)) + (1.0f - O) * B);
}

static float blend_exclusion_f(const float B, const float L, float O)
{	
	return (float)(O * (B + L - 2 * B * L / 255) + (1.0f - O) * B);
}

static float blend_overlay_f(const float B, const float L, float O)
{	
	return (float)(O * ((L < 128) ? (2 * B * L / 255) : (255 - 2 * (255 - B) * (255 - L) / 255)) + (1.0f - O) * B);
}

static float blend_soft_light_f(const float B, const float L, float O)
{	
	return (float)(O * ((L < 128) ? (2 * ((B >> 1) + 64)) * ((float)L / 255) : (255 - (2 * (255 - ((B >> 1) + 64)) * (float)(255 - L) / 255))) + (1.0f - O) * B);
}

static float blend_hard_light_f(const float B, const float L, float O)
{	
	return (float)(O * ((B < 128) ? (2 * L * B / 255) : (255 - 2 * (255 - L) * (255 - B) / 255)) + (1.0f - O) * B);
}

static float blend_color_dodge_f(const float B, const float L, float O)
{	
	return (float)(O * ((L == 255) ? L : minf(255, ((B << 8 ) / (255 - L)))) + (1.0f - O) * B);
}

static float blend_color_burn_f(const float B, const float L, float O)
{	
	return (float)(O * ((L == 0) ? L : maxf(0, (255 - ((255 - B) << 8 ) / L))) + (1.0f - O) * B);
}

static float blend_linear_dodge_f(const float B, const float L, float O)
{	
	return (float)(O * (minf(255, (B + L))) + (1.0f - O) * B);
}

static float blend_linear_burn_f(const float B, const float L, float O)
{	
	return (float)(O * ((B + L < 255) ? 0 : (B + L - 255)) + (1.0f - O) * B);
}

static float blend_linear_light_f(const float B, const float L, float O)
{	
	return (float)(O * ((2 * L) < 128) ? ((B + (2 * L) < 255) ? 0 : (B + (2 * L) - 255)) : (minf(255, (B + (2 * (L - 128))))) + (1.0f - O) * B);
}

static float blend_vivid_light_f(const float B, const float L, float O)
{	
	return (float)(O * (L < 128) ? (((2 * L) == 0) ? (2 * L) : maxf(0, (255 - ((255 - B) << 8 ) / (2 * L)))) : (minf(255, (B + (2 * (L - 128))))) + (1.0f - O) * B);
}

static float blend_pin_light_f(const float B, const float L, float O)
{	
	return (float)(O * (L < 128) ? (((2 * L) > B) ? B : (2 * L)) : (((2 * (L - 128)) > B) ? (2 * (L - 128)) : B) + (1.0f - O) * B);
}

static float blend_hard_mix_f(const float B, const float L, float O)
{	
	return (float)(O * (((L < 128) ? (((2 * L) == 0) ? (2 * L) : maxf(0, (255 - ((255 - B) << 8 ) / (2 * L)))) : (minf(255, (B + (2 * (L - 128))))) < 128) ? 0 : 255) + (1.0f - O) * B);
}*/

/*static float blend_normal_f(const float B, const float L, const float O)
{	
	return (char)(O * (L) + (1.0f - O) * B);
}

static float blend_multiply_f(const float B, const float L, const float O)
{	
	return (float)(O * ((B * L) / 1.0f) + (1.0f - O) * B);
}

static float blend_subtract_f(const float B, const float L, const float O)
{	
	return (char)(O * ((B + L < 255) ? 0 : (B + L - 255)) + (1.0f - O) * B);
}

static float blend_difference_f(const float B, const float L, const float O)
{	
	return (float)(O * (fabs(B - L)) + (1.0f - O) * B);
}

static float blend_overlay_f(float B, float L, float O)
{	
	return (float)(O * ((L < 0.5f) ? (2 * B * L / 1.0f) : (1.0f - 2 * (1.0f - B) * (1.0f - L) / 1.0f)) + (1.0f - O) * B);
}*/

static char pixel_is_transparent(const char pix[4])
{	
	if ((pix[0] == 0) && (pix[1] == 0) && (pix[2] == 0) && (pix[3] == 0))
		return 1;
	return 0;
}

ImBuf *imalayer_blend(ImBuf *base, ImBuf *layer, float opacity, short mode)
{
	ImBuf *dest;
	int i, y;
	int bg_x, bg_y, diff_x;
	//float (*blend_callback_f)(const float B, const float L, const float O) = NULL;	//Mode callback
	char (*blend_callback)(const char B, const char L, const float O) = NULL;		//Mode callback
	
	if (!base)
		return IMB_dupImBuf(layer);

	dest = IMB_dupImBuf(base);

	if (opacity == 0.0f)
		return dest;

	bg_x = base->x;
	bg_y = base->y;
	if (base->x > layer->x)
		bg_x = layer->x;
	if (base->y > layer->y)
		bg_y = layer->y;
	
	diff_x = abs(base->x - layer->x);

	switch(mode) {
	case IMA_LAYER_NORMAL:
		blend_callback = blend_normal;
		break;

	case IMA_LAYER_MULTIPLY:
		blend_callback = blend_multiply;
		break;

	case IMA_LAYER_SCREEN:
		blend_callback = blend_screen;
		break;

	case IMA_LAYER_OVERLAY:
		blend_callback = blend_overlay;
		break;

	case IMA_LAYER_SOFT_LIGHT:
		blend_callback = blend_soft_light;
		break;

	case IMA_LAYER_HARD_LIGHT:
		blend_callback = blend_hard_light;
		break;

	case IMA_LAYER_COLOR_DODGE:
		blend_callback = blend_color_dodge;
		break;

	case IMA_LAYER_LINEAR_DODGE:
		blend_callback = blend_linear_dodge;
		break;
		
	case IMA_LAYER_COLOR_BURN:
		blend_callback = blend_color_burn;
		break;

	case IMA_LAYER_LINEAR_BURN:
		blend_callback = blend_linear_burn;
		break;

	case IMA_LAYER_AVERAGE: 
		blend_callback = blend_average;
		break;       

	case IMA_LAYER_ADD: 
		blend_callback = blend_add;
		break;

	case IMA_LAYER_SUBTRACT: 
		blend_callback = blend_subtract;
		break;       

	case IMA_LAYER_DIFFERENCE: 
		blend_callback = blend_difference;
		break;
	
	case IMA_LAYER_LIGHTEN: 
		blend_callback = blend_lighten;
		break;       

	case IMA_LAYER_DARKEN: 
		blend_callback = blend_darken;
		break;
	
	case IMA_LAYER_NEGATION: 
		blend_callback = blend_negation;
		break;       

	case IMA_LAYER_EXCLUSION: 
		blend_callback = blend_exclusion;
		break;
	
	case IMA_LAYER_LINEAR_LIGHT: 
		blend_callback = blend_linear_light;
		break;       

	case IMA_LAYER_VIVID_LIGHT: 
		blend_callback = blend_vivid_light;
		break;
	
	case IMA_LAYER_PIN_LIGHT: 
		blend_callback = blend_pin_light;
		break;       

	case IMA_LAYER_HARD_MIX: 
		blend_callback = blend_hard_mix;
		break;
	}

	if (base->rect_float) {
		
		float *fp_b = (float *) base->rect_float;
		float *fp_l = (float *) layer->rect_float;
		float *fp_d = (float *) dest->rect_float;
		for( i = bg_x * bg_y; i > 0; i--, fp_b+=4, fp_l+=4, fp_d+=4 ) {
			if (fp_l[3] != 0.0f) {
				fp_d[0] = (float) blend_callback(FTOCHAR(fp_b[0]), FTOCHAR(fp_l[0]), opacity) / 255.0f;
				fp_d[1] = (float) blend_callback(FTOCHAR(fp_b[1]), FTOCHAR(fp_l[1]), opacity) / 255.0f;
				fp_d[2] = (float) blend_callback(FTOCHAR(fp_b[2]), FTOCHAR(fp_l[2]), opacity) / 255.0f;
			}
		}

		if(base->rect) {
			IMB_rect_from_float(base);
		}
	}
	else if(base->rect) {
		
		char *cp_b = (char *) base->rect;
		char *cp_l = (char *) layer->rect;
		char *cp_d = (char *) dest->rect;
		for( i = bg_x * bg_y; i > 0; i--) {
			if ((!(pixel_is_transparent(cp_b))) && (!(pixel_is_transparent(cp_l)))) {
				if ((cp_l[3] == 255) && (cp_b[3] == 255)) {
					cp_d[0] = blend_callback(cp_b[0], cp_l[0], opacity);
					cp_d[1] = blend_callback(cp_b[1], cp_l[1], opacity);
					cp_d[2] = blend_callback(cp_b[2], cp_l[2], opacity);
				}
				else if ((cp_l[3] != 255) || (cp_b[3] != 255)) {
					cp_d[0] = blend_callback(cp_b[0], cp_l[0], opacity);
					cp_d[1] = blend_callback(cp_b[1], cp_l[1], opacity);
					cp_d[2] = blend_callback(cp_b[2], cp_l[2], opacity);
					/*if ((cp_b[3] != 0) && (cp_l[3] != 0))*/
						cp_d[3] = blend_callback(cp_b[3], cp_l[3], opacity);
				}
				else if (cp_l[3] != 0) {
					cp_d[0] = cp_l[0];
					cp_d[1] = cp_l[1];
					cp_d[2] = cp_l[2];
					cp_d[3] = cp_l[3];
				}
				else {
					cp_d[0] = cp_b[0];
					cp_d[1] = cp_b[1];
					cp_d[2] = cp_b[2];
					cp_d[3] = cp_b[3];
				}
			}
			if (((i % bg_x) == 0) && (i != (bg_x * bg_y))) {
				if (base->x > layer->x) {
					cp_b = cp_b + diff_x*4;
					cp_d = cp_d + diff_x*4;
				}
				else
					cp_l = cp_l + diff_x*4;
			}
			cp_b+=4;
			cp_l+=4; 
			cp_d+=4;
		}
	}

	return dest;
}

struct ImageLayer *merge_layers(Image *ima, ImageLayer *iml, ImageLayer *iml_next)
{
	ImBuf *ibuf, *result_ibuf;
	 /* merge layers */
	result_ibuf = imalayer_blend((ImBuf*)((ImageLayer*)iml_next->ibufs.first), (ImBuf*)((ImageLayer*)iml->ibufs.first), iml->opacity, iml->mode);
	
	iml_next->background = IMA_LAYER_BG_RGB;
	iml_next->file_path[0] = '\0';
	iml_next->default_color[0] = -1;
	iml_next->default_color[1] = -1;
	iml_next->default_color[2] = -1;
	iml_next->default_color[3] = -1;

	/* Delete old ibuf*/
	ibuf = (ImBuf *)iml_next->ibufs.first;
	BLI_remlink(&iml_next->ibufs, ibuf);
 
	if (ibuf->userdata) {
		MEM_freeN(ibuf->userdata);
		ibuf->userdata = NULL;
	}
 	IMB_freeImBuf(ibuf);

	/* add new ibuf */
	BLI_addtail(&iml_next->ibufs, result_ibuf);

	/* delete the layer merge */
	BLI_remlink(&ima->imlayers, iml);
	free_image_layer(iml);
	
	return iml_next;
}
/* Non distruttivo */
void merge_layers_visible_nd(Image *ima)
{
	ImageLayer *layer, *next;
	ImBuf *ibuf, *result_ibuf, *next_ibuf, *old;
	void *lock;
	//printf("1\n");
	result_ibuf = NULL;
	
	for(layer=(ImageLayer *)ima->imlayers.last; layer; layer=layer->prev) {
		if (layer->visible & IMA_LAYER_VISIBLE) {
			ibuf = (ImBuf*)((ImageLayer*)layer->ibufs.first);
				
			if (ibuf) {
				next_ibuf = imalayer_blend(result_ibuf, ibuf, layer->opacity, layer->mode);
				if (result_ibuf)
					IMB_freeImBuf(result_ibuf);
				
				result_ibuf = IMB_dupImBuf(next_ibuf);
				if (next_ibuf)
					IMB_freeImBuf(next_ibuf);
			}
		}
	}
	old = (ImBuf *)ima->ibufs.first;
	//result_ibuf->profile = IB_PROFILE_LINEAR_RGB;
	ima->ibufs.first = result_ibuf;
	if (old)
		IMB_freeImBuf(old);
	//printf("2\n");
}

static int imlayer_find_name_dupe(const char *name, ImageLayer *iml, Image *ima)
{
	ImageLayer *layer;

	for (layer = ima->imlayers.last; layer; layer=layer->prev) {
		if (iml!=layer) {
			if (!strcmp(layer->name, name)) {
				return 1;
			}
		}
	}

	return 0;
}

static int imlayer_unique_check(void *arg, const char *name)
{
	struct {Image *ima; void *iml;} *data= arg;
	return imlayer_find_name_dupe(name, data->iml, data->ima);
}

void imalayer_unique_name(ImageLayer *iml, Image *ima)
{
	struct {Image *ima; void *iml;} data;
	data.ima = ima;
	data.iml = iml;

	BLI_uniquename_cb(imlayer_unique_check, &data, "Layer", '.', iml->name, sizeof(iml->name));
}

ImageLayer *image_add_image_layer(Image *ima, const char *name, int depth, float color[4], int order)
{
	ImageLayer *layer, *layer_act, *im_l = NULL;
	ImBuf *ibuf, *imaibuf;
	void *lock;
 
	if(ima==NULL)
		return NULL;

 	layer_act = imalayer_get_current(ima);

	/* Deselect other layers */
	/*for(layer = ima->imlayers.first; layer; layer=layer->next)
		layer->select = !IMA_LAYER_SEL_CURRENT;*/
	layer_act->select = !IMA_LAYER_SEL_CURRENT;
	
	im_l = layer_alloc(ima, name);
	if (im_l) {
		ibuf= BKE_image_acquire_ibuf(ima, NULL, &lock, IMA_IBUF_IMA);
 
		imaibuf = (ImBuf*)ima->ibufs.first;
		ibuf = add_ibuf_size(imaibuf->x, imaibuf->y, im_l->name, depth, ima->gen_flag, 0, color);
		BLI_addtail(&im_l->ibufs, ibuf);
		if (order == 2) { /*Head*/
			BLI_addhead(&ima->imlayers, im_l);
			ima->Act_Layers = 0;
		}
		else if (order == -1) { /*Before*/
			/* Layer Act
			 * --> Add Layer
			 */
			BLI_insertlinkafter(&ima->imlayers, layer_act , im_l);
			ima->Act_Layers += 1;
			imalayer_set_current_act(ima, ima->Act_Layers);
		}
		else { /*After*/
			/* --> Add Layer
			 * Layer Act
			 */
			BLI_insertlinkbefore(&ima->imlayers, layer_act , im_l);
			//ima->Act_Layers -= 1;
			imalayer_set_current_act(ima, ima->Act_Layers);
		}
		ima->Count_Layers += 1;

		if (color[3] == 1.0f)
			im_l->background = IMA_LAYER_BG_RGB;
		copy_v4_v4(im_l->default_color, color);
		BKE_image_release_ibuf(ima, lock);
	}
 
	return im_l;
}
 
/* TODO: (kwk) Base image layer needs proper locking... */
void image_add_image_layer_base(Image *ima)
{
	ImageLayer *im_l = NULL;
	ImBuf *ibuf;
	void *lock;
 
	if(ima) {
		im_l = layer_alloc(ima, "Background");
		if (im_l) {	
			ibuf= BKE_image_acquire_ibuf(ima, NULL, &lock, IMA_IBUF_IMA);
 			im_l->type = IMA_LAYER_BASE; /* BASE causes no free on deletion of layer */
			ima->Act_Layers = 0;
			ima->Count_Layers = 1;
			/* Get ImBuf from ima */
			ibuf = IMB_dupImBuf((ImBuf*)ima->ibufs.first);
			BLI_addtail(&im_l->ibufs, ibuf);
			BLI_addhead(&ima->imlayers, im_l);
 
			BKE_image_release_ibuf(ima, lock);
		}
	}
}
