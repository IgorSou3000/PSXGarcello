/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "weekga2.h"

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
	Gfx_Tex tex_back2; //Body
} Back_Week8a2;

//Week 5 background functions
void Back_Week8a2_DrawBG(StageBack *back)
{
	Back_Week8a2 *this = (Back_Week8a2*)back;

		fixed_t fx, fy;

	//Draw Garcello Body
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	static const struct Back_Week8a2_BodyPiece
	{
		RECT src;
		fixed_t scale;
	} body_piece[] = {
		{{  0, 0, 0, 0}, FIXED_DEC(1,1)},
	};
	
	RECT_FIXED body_dst = {
		FIXED_DEC(-0,1) - fx,
		FIXED_DEC(-0,1) - fy,
		0,
		FIXED_DEC(0,1)
	};
	
	RECT body_src = {0, 0,116,64};
	RECT_FIXED body_fill;
	body_fill.x = FIXED_DEC(-190,1) - fx,
	body_fill.y = FIXED_DEC(20,1) - fy,
	body_fill.w = FIXED_DEC(116,1);
	body_fill.h = FIXED_DEC(64,1);
	Stage_DrawTex(&this->tex_back2, &body_src, &body_fill, stage.camera.bzoom);
	
	const struct Back_Week8a2_BodyPiece *body_p = body_piece;
	for (size_t i = 0; i < COUNT_OF(body_piece); i++, body_p++)
	{
		body_dst.w = body_p->src.w ? (body_p->src.w * body_p->scale) : body_p->scale;
		Stage_DrawTex(&this->tex_back2, &body_p->src, &body_dst, stage.camera.bzoom);
		body_dst.x += body_dst.w;
	}

	//Draw Floor
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	static const struct Back_Week8a2_FloorPiece
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
	
	const struct Back_Week8a2_FloorPiece *floor_p = floor_piece;
	for (size_t i = 0; i < COUNT_OF(floor_piece); i++, floor_p++)
	{
		floor_dst.w = floor_p->src.w ? (floor_p->src.w * floor_p->scale) : floor_p->scale;
		Stage_DrawTex(&this->tex_back0, &floor_p->src, &floor_dst, stage.camera.bzoom);
		floor_dst.x += floor_dst.w;
	}

	//Draw Fence
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	static const struct Back_Week8a2_FencePiece
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
	fence_fill.y = FIXED_DEC(-215,1) - fy,
	fence_fill.w = FIXED_DEC(415,1);
	fence_fill.h = FIXED_DEC(280,1);
	Stage_DrawTex(&this->tex_back1, &fence_src, &fence_fill, stage.camera.bzoom);
	
	const struct Back_Week8a2_FencePiece *fence_p = fence_piece;
	for (size_t i = 0; i < COUNT_OF(fence_piece); i++, fence_p++)
	{
		fence_dst.w = fence_p->src.w ? (fence_p->src.w * fence_p->scale) : fence_p->scale;
		Stage_DrawTex(&this->tex_back1, &fence_p->src, &fence_dst, stage.camera.bzoom);
		fence_dst.x += fence_dst.w;
	}
	
	//Draw back wall
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	static const struct Back_Week8a2_WallPiece
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
	
	const struct Back_Week8a2_WallPiece *wall_p = wall_piece;
	for (size_t i = 0; i < COUNT_OF(wall_piece); i++, wall_p++)
	{
		wall_dst.w = wall_p->src.w ? (wall_p->src.w * wall_p->scale) : wall_p->scale;
		Stage_DrawTex(&this->tex_back0, &wall_p->src, &wall_dst, stage.camera.bzoom);
		wall_dst.x += wall_dst.w;
	}
}

void Back_Week8a2_Free(StageBack *back)
{
	Back_Week8a2 *this = (Back_Week8a2*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week8a2_New()
{
	//Allocate background structure
	Back_Week8a2 *this = (Back_Week8a2*)Mem_Alloc(sizeof(Back_Week8a2));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week8a2_DrawBG;
	this->back.free = Back_Week8a2_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK8A2\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "back2.tim"), 0);
	
	return (StageBack*)this;
}
