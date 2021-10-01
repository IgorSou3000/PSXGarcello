#include "gclt.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Garcello character structure
enum
{
	Gclt_ArcMain_Idle0,
	Gclt_ArcMain_Idle1,
	Gclt_ArcMain_Left0,
	Gclt_ArcMain_Left1,
	Gclt_ArcMain_Down0,
    Gclt_ArcMain_Down1,
	Gclt_ArcMain_Downa0,
    Gclt_ArcMain_Downa1,
	Gclt_ArcMain_Up0,
	Gclt_ArcMain_Up1,
	Gclt_ArcMain_Right0,
	Gclt_ArcMain_Right1,
	
	Gclt_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Gclt_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Gclt;

//Garcello Death character definitions
static const CharFrame char_Gclt_frame[] = {
	{Gclt_ArcMain_Idle0, {  0,   0,   80, 162}, { 49, 155}}, //0 idle 1
	{Gclt_ArcMain_Idle0, { 80,   0,   80, 162}, { 47, 155}}, //1 idle 2
	{Gclt_ArcMain_Idle1, {  0,   0,   79, 162}, { 49, 156}}, //2 idle 3
	{Gclt_ArcMain_Idle1, { 79,   0,   80, 162}, { 48, 156}}, //3 idle 4
	
	{Gclt_ArcMain_Left0, {  0,   0,  90,162}, { 54,  155}}, //4 left 1
	{Gclt_ArcMain_Left0, { 90,   0,  90,162}, { 54,  155}}, //5 left 2
	{Gclt_ArcMain_Left1, {  0,   0,  90,162}, { 54,  155}}, //4 left 1
	{Gclt_ArcMain_Left1, { 90,   0,  90,162}, { 54,  155}}, //5 left 2
	
	{Gclt_ArcMain_Down0, {  0,   0,  90, 158}, { 44, 149}}, //6 down 1
	{Gclt_ArcMain_Down0, { 90,   0,  90, 158}, { 41, 149}}, //7 down 2
	{Gclt_ArcMain_Down1, {  0,   0,  91, 157}, { 44, 149}}, //6 down 1
	{Gclt_ArcMain_Down1, { 91,   0,  89, 157}, { 41, 150}}, //7 down 2

	{Gclt_ArcMain_Downa0, {  0,   0,  85, 163}, { 48, 155}}, //6 cough 1
	{Gclt_ArcMain_Downa0, { 85,   0,  85, 163}, { 48, 155}}, //7 cough 2
	{Gclt_ArcMain_Downa1, {  0,   0,  85, 165}, { 50, 155}}, //6 cough 1
	{Gclt_ArcMain_Downa1, { 85,   0,  85, 165}, { 50, 155}}, //7 cough 2
	
	{Gclt_ArcMain_Up0, {  0,   0,  94, 170}, { 46, 162}}, //8 up 1
	{Gclt_ArcMain_Up0, { 94,   0,  94, 170}, { 46, 162}}, //9 up 2
	{Gclt_ArcMain_Up1, {  0,   0,  96, 170}, { 46, 160}}, //8 up 1
	{Gclt_ArcMain_Up1, { 96,   0,  90, 170}, { 46, 160}}, //9 up 2

	{Gclt_ArcMain_Right0, {  0,   0,  96, 164}, { 47, 157}}, //10 right 1
	{Gclt_ArcMain_Right0, { 96,   0,  96, 164}, { 47, 157}}, //11 right 2
	{Gclt_ArcMain_Right1, {  0,   0,  93, 164}, { 47, 157}}, //10 right 1
	{Gclt_ArcMain_Right1, { 93,   0,  93, 164}, { 47, 157}}, //11 right 2
};

 static const Animation char_gclt_anim[CharAnim_Max] = {
	{2, (const u8[]){ 1,  2,  3,  0, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 4,  5,  6,  7, ASCR_BACK, 1}},         //CharAnim_Left
	{2, (const u8[]){ 4,  5,  6,  7, ASCR_BACK, 1}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 8,  9, 10, 11, ASCR_BACK, 1}},         //CharAnim_Down
	{2, (const u8[]){12, 13, 14, 15, ASCR_BACK, 1}},   //CharAnim_Cough
	{2, (const u8[]){16, 17, 18, 19, ASCR_BACK,  1}},         //CharAnim_Up
	{2, (const u8[]){16, 17, 18, 19, ASCR_BACK,  1}},   //CharAnim_UpAlt
	{2, (const u8[]){20, 21, 22, 23, ASCR_BACK,  1}},         //CharAnim_Right
	{2, (const u8[]){20, 21, 22, 23,ASCR_BACK,  1}},   //CharAnim_RightAlt
};

//Gclt character functions
void Char_Gclt_SetFrame(void *user, u8 frame)
{
	Char_Gclt *this = (Char_Gclt*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_Gclt_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Gclt_Tick(Character *character)
{
	Char_Gclt *this = (Char_Gclt*)character;
	
	//Perform idle dance
	Character_CheckEndSing(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		if ((Animatable_Ended(&character->animatable) || character->animatable.anim == CharAnim_LeftAlt  || character->animatable.anim == CharAnim_UpAlt  || character->animatable.anim == CharAnim_DownAlt || character->animatable.anim == CharAnim_RightAlt) &&
		   (character->animatable.anim != CharAnim_Left &&
		     character->animatable.anim != CharAnim_LeftAlt &&
		     character->animatable.anim != CharAnim_Down &&
		     character->animatable.anim != CharAnim_DownAlt &&
		     character->animatable.anim != CharAnim_Up &&
		     character->animatable.anim != CharAnim_UpAlt &&
		     character->animatable.anim != CharAnim_Right &&
		     character->animatable.anim != CharAnim_RightAlt) &&
		    (stage.song_step & 0x3) == 0)
			character->set_anim(character, CharAnim_Idle);
	}
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Gclt_SetFrame);
	Character_Draw(character, &this->tex, &char_Gclt_frame[this->frame]);
}


void Char_Gclt_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Gclt_Free(Character *character)
{
	Char_Gclt *this = (Char_Gclt*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Gclt_New(fixed_t x, fixed_t y)
{
	//Allocate Gclt object
	Char_Gclt *this = Mem_Alloc(sizeof(Char_Gclt));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Gclt_New] Failed to allocate Gclt object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Gclt_Tick;
	this->character.set_anim = Char_Gclt_SetAnim;
	this->character.free = Char_Gclt_Free;
	
	Animatable_Init(&this->character.animatable, char_gclt_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character stage information
	this->character.health_i = 12;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-130,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\GCLT.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Gclt_ArcMain_Idle0
		"idle1.tim", //Gclt_ArcMain_Idle1
		"left0.tim",  //Gclt_ArcMain_Left
		"left1.tim",  //Gclt_ArcMain_Left
		"down0.tim",  //Gclt_ArcMain_Down
		"down1.tim",  //Gclt_ArcMain_Down
		"downa0.tim", //Gclt_ArcMain_Down Alt
		"downa1.tim", //Gclt_ArcMain_Down Alt
		"up0.tim",    //Gclt_ArcMain_Up
		"up1.tim",    //Gclt_ArcMain_Up
		"right0.tim", //Gclt_ArcMain_Right
		"right1.tim", //Gclt_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);

	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
