/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "weekg.h"

#include "../archive.h"
#include "../mem.h"

//Week 5 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Back wall
	Gfx_Tex tex_back1; //Second floor
} Back_Week8;

//Week 5 background functions
void Back_Week8_DrawBG(StageBack *back)
{
	Back_Week8 *this = (Back_Week8*)back;

		fixed_t fx, fy;

	//Draw Floor
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	static const struct Back_Week8_FloorPiece
	{
		RECT src;
		fixed_t scale;
	} floor_piece[] = {
		{{  0, 0, 0, 0}, FIXED_DEC(1,1)},
	};
	
	RECT_FIXED floor_dst = {
		FIXED_DEC(-0,1) - fx,
		FIXED_DEC(-0,1) - fy,
		0,
		FIXED_DEC(0,1)
	};
	
	RECT floor_src = {0, 0,256,58};
	RECT_FIXED floor_fill;
	floor_fill.x = FIXED_DEC(-200,1) - fx,
	floor_fill.y = FIXED_DEC( 60,1) - fy,
	floor_fill.w = FIXED_DEC(400,1);
	floor_fill.h = FIXED_DEC(110,1);
	Stage_DrawTex(&this->tex_back0, &floor_src, &floor_fill, stage.camera.bzoom);
	
	const struct Back_Week8_FloorPiece *floor_p = floor_piece;
	for (size_t i = 0; i < COUNT_OF(floor_piece); i++, floor_p++)
	{
		floor_dst.w = floor_p->src.w ? (floor_p->src.w * floor_p->scale) : floor_p->scale;
		Stage_DrawTex(&this->tex_back0, &floor_p->src, &floor_dst, stage.camera.bzoom);
		floor_dst.x += floor_dst.w;
	}

	//Draw Fence
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	static const struct Back_Week8_FencePiece
	{
		RECT src;
		fixed_t scale;
	} fence_piece[] = {
		{{  0, 0, 0, 0}, FIXED_DEC(1,1)},
	};
	
	RECT_FIXED fence_dst = {
		FIXED_DEC(-0,1) - fx,
		FIXED_DEC(-0,1) - fy,
		0,
		FIXED_DEC(0,1)
	};
	
	RECT fence_src = {0, 0,256,103};
	RECT_FIXED fence_fill;
	fence_fill.x = FIXED_DEC(-222,1) - fx,
	fence_fill.y = FIXED_DEC(-140,1) - fy,
	fence_fill.w = FIXED_DEC(415,1);
	fence_fill.h = FIXED_DEC(200,1);
	Stage_DrawTex(&this->tex_back1, &fence_src, &fence_fill, stage.camera.bzoom);
	
	const struct Back_Week8_FencePiece *fence_p = fence_piece;
	for (size_t i = 0; i < COUNT_OF(fence_piece); i++, fence_p++)
	{
		fence_dst.w = fence_p->src.w ? (fence_p->src.w * fence_p->scale) : fence_p->scale;
		Stage_DrawTex(&this->tex_back1, &fence_p->src, &fence_dst, stage.camera.bzoom);
		fence_dst.x += fence_dst.w;
	}
	
	//Draw back wall
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	static const struct Back_Week8_WallPiece
	{
		RECT src;
		fixed_t scale;
	} wall_piece[] = {
		{{  0, 0, 0, 0}, FIXED_DEC(2,1)},
	};
	
	RECT_FIXED wall_dst = {
		FIXED_DEC(-0,1) - fx,
		FIXED_DEC(-0,1) - fy,
		0,
		FIXED_DEC(0,1)
	};
	
	RECT wall_src = {0, 58, 256,199};
	RECT_FIXED wall_fill;
	wall_fill.x = FIXED_DEC(-222,1) - fx,
	wall_fill.y = FIXED_DEC(-170,1) - fy,
	wall_fill.w = FIXED_DEC(415,1);
	wall_fill.h = FIXED_DEC(310,1);
	Stage_DrawTex(&this->tex_back0, &wall_src, &wall_fill, stage.camera.bzoom);
	
	const struct Back_Week8_WallPiece *wall_p = wall_piece;
	for (size_t i = 0; i < COUNT_OF(wall_piece); i++, wall_p++)
	{
		wall_dst.w = wall_p->src.w ? (wall_p->src.w * wall_p->scale) : wall_p->scale;
		Stage_DrawTex(&this->tex_back0, &wall_p->src, &wall_dst, stage.camera.bzoom);
		wall_dst.x += wall_dst.w;
	}
}

void Back_Week8_Free(StageBack *back)
{
	Back_Week8 *this = (Back_Week8*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week8_New()
{
	//Allocate background structure
	Back_Week8 *this = (Back_Week8*)Mem_Alloc(sizeof(Back_Week8));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week8_DrawBG;
	this->back.free = Back_Week8_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK8\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	
	return (StageBack*)this;
}
