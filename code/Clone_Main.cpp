#include "Clone_Main.h"

internal Animation
CreateAnimation(int Init, int End, int Current, int Time, int Delay) 
{
    Animation Animation;

    Animation.StartingFrame = Init;
    Animation.EndingFrame = End;
    Animation.CurrentFrame = Current;
    Animation.FrameTime = Time;
    Animation.FrameDelay = Delay;

    return(Animation);
}

internal void 
HandlePlayerInput(State *State) 
{
    // TODO : Replace this with the State->Input struct's functionality

    if(IsKeyDown(KEY_W)) 
    {
        State->Player.Vel.y = -State->Player.MovementSpeed;
    }
    else if(IsKeyDown(KEY_S)) 
    {
        State->Player.Vel.y = State->Player.MovementSpeed;
    }

    if(IsKeyDown(KEY_A)) 
    {
        State->Player.Vel.x = -State->Player.MovementSpeed;
        State->Player.Flags.Flipped = true;
    }
    else if(IsKeyDown(KEY_D)) 
    {
        State->Player.Vel.x = State->Player.MovementSpeed;
        State->Player.Flags.Flipped = false;
    }

    if(IsKeyUp(KEY_W) && IsKeyUp(KEY_S)) 
    {
        State->Player.Vel.y = 0;
    }

    if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) 
    {
        State->Player.Vel.x = 0;
    }

    if(IsKeyUp(KEY_W) & IsKeyUp(KEY_S) & IsKeyUp(KEY_A) & IsKeyUp(KEY_D)) 
    {
        State->Player.AnimationState = IDLE;
    }

    State->Player.Pos.x += State->Player.Vel.x * GetFrameTime();
    State->Player.Pos.y += State->Player.Vel.y * GetFrameTime();

    State->Player.DstRect = 
    {   
        State->Player.Pos.x, 
        State->Player.Pos.y, 
        State->Player.SrcRect.width,
        State->Player.SrcRect.height
    };
}

void 
PlayAnimation(Entity *Entity) 
{
    switch(Entity->EntityType) 
    {
        case PLAYER: 
        {
            switch(Entity->AnimationState) 
            {
                case IDLE: 
                {
                    ++Entity->IdleAnimation.FrameTime;
                    if(Entity->IdleAnimation.FrameTime >= Entity->IdleAnimation.FrameDelay) 
                    {
                        ++Entity->IdleAnimation.CurrentFrame;
                        Entity->IdleAnimation.FrameTime = 0;
                    }

                    if(Entity->IdleAnimation.CurrentFrame > Entity->IdleAnimation.EndingFrame) 
                    {
                        Entity->IdleAnimation.CurrentFrame = Entity->IdleAnimation.StartingFrame;
                    }
                    
                    Entity->SrcRect = 
                    {
                        real32((Entity->Sprite.width / Entity->SpriteFrames) * Entity->IdleAnimation.CurrentFrame),
                        0,
                        real32(Entity->Sprite.width / Entity->SpriteFrames),
                        real32(Entity->Sprite.height),
                    };
                    // TODO : Gross, Maybe find a better way to reset different animation's frames
                    Entity->WalkingAnimation.CurrentFrame = Entity->WalkingAnimation.StartingFrame;
                }break;
                case WALKING:
                {
                    ++Entity->WalkingAnimation.FrameTime;
                    if(Entity->WalkingAnimation.FrameTime > Entity->WalkingAnimation.FrameDelay) 
                    {
                        ++Entity->WalkingAnimation.CurrentFrame;
                        Entity->WalkingAnimation.FrameTime = 0;
                    }

                    if(Entity->WalkingAnimation.CurrentFrame >= Entity->WalkingAnimation.EndingFrame) 
                    {
                        Entity->WalkingAnimation.CurrentFrame = Entity->WalkingAnimation.StartingFrame;
                    }
                    
                    Entity->SrcRect = 
                    {
                        real32((Entity->Sprite.width / Entity->SpriteFrames) * Entity->WalkingAnimation.CurrentFrame),
                        0,
                        real32(Entity->Sprite.width / Entity->SpriteFrames),
                        real32(Entity->Sprite.height),
                    };
                    // TODO : Same as above here, perhaps there's a better way to reset 
                    Entity->IdleAnimation.CurrentFrame = Entity->IdleAnimation.StartingFrame;
                }break;
                case JUMPING:
                {
                }break;
            }
        }break;
        case ENEMY: 
        {
            switch(Entity->AnimationState)
            {
                case IDLE: 
                {
                }break;
                case WALKING:
                {
                }break;
                default: 
                {
                }break;
            }
        }
        case MAPOBJECT: 
        {
            switch(Entity->AnimationState) 
            {
                case IDLE: 
                {
                }break;
                default: 
                {
                }break;
            }
        }break;
    }

    if(Entity->Flags.Flipped) 
    {
        Entity->SrcRect.width = -Entity->SrcRect.width;
        Entity->DstRect.width = -Entity->SrcRect.width;
    }
    else 
    {
        Entity->SrcRect.width = Entity->SrcRect.width;
        Entity->DstRect.width = Entity->SrcRect.width;
    }
}

internal void 
HandleCamera(State *State) 
{
    const int PixelHeightPerScreen = 720;

    State->Camera.zoom = real32(GetScreenWidth() / PixelHeightPerScreen);
    State->Camera.offset = {real32(GetScreenWidth() / 2), real32(GetScreenHeight() / 2)};
    State->Camera.target = {State->Player.Pos.x, State->Player.Pos.y};
    State->Camera.rotation = 0;
}

internal void 
HandleAnimationStateMachine(Entity *Entity) 
{   
    if(Vec2EqualsS(Entity->Vel, 0)) 
    {
        Entity->Flags.IsMoving = false;
        Entity->AnimationState = IDLE;
    }
    else 
    {
        Entity->Flags.IsMoving = true;
        Entity->AnimationState = WALKING;
    }

    PlayAnimation(Entity);
}

const int MAPSCALE = 2;
const int TILESIZE = 16;

internal LevelGridData
LoadLevelData(const char *Filepath) 
{
    LevelGridData Grid = {};    

    FILE *File = fopen(Filepath, "rt");
    if(File == nullptr) 
    {
        printf("Failed to load the file: %s\n", Filepath);
    }
    
    int RowCount = 0;
    char Buffer[1024];
    while(fgets(Buffer, 1024, File) != NULL) 
    {
        ++RowCount;
    }
    fseek(File, 0, SEEK_SET);

    int ColumnCount = 0;
    fgets(Buffer, 1024, File);

    for (int i = 0; Buffer[i] != '\0'; ++i) 
    {
        if (Buffer[i] != '\n') 
        {
            ++ColumnCount;
        }
    }
    ++ColumnCount;

    Grid.LevelGrid = (char **)malloc(RowCount * sizeof(char *));
    for(int i = 0; i < RowCount; ++i) 
    {
        Grid.LevelGrid[i] = (char *)malloc(ColumnCount * sizeof(char));
        memset(Grid.LevelGrid[i], '\0', ColumnCount);
    }

    for(int i = 0; i < RowCount; ++i) 
    {
        fgets(Buffer, 1024, File);
        int k = 0;
        for(int j = 0; Buffer[j] != '\0'; ++j) 
        {
            Grid.LevelGrid[i][k++] = Buffer[j];
        }
    }
    fclose(File);

    Grid.LevelSize = {RowCount, ColumnCount};

    printf("C: %i\nR: %i", Grid.LevelSize.x, Grid.LevelSize.y);
    return Grid;
}

internal Level
CreateLevel(State *State) 
{
    Level Level;

    Level.LevelImage = State->Textures[1];
    Level.LevelGridData = 
        LoadLevelData("../data/res/mapdata/Test/simplified/AutoLayers_advanced_demo/IntGrid_layer.csv");
    Level.SrcRect = 
        {0, 0, real32(Level.LevelImage.width), real32(Level.LevelImage.height)};
    Level.DstRect = 
        {0, 0, real32(Level.SrcRect.width * MAPSCALE), real32(Level.SrcRect.height * MAPSCALE)};

    Level.IsLoaded = true;
    Level.Index = 1;
    Level.EntityCount = 0;
    Level.Entities = (Entity *)malloc(int(sizeof(Entity)) * Level.EntityCount);

    return(Level);
}

internal void 
DrawMap(Level *Level) 
{   
    int CommaOffset = 0;

    for(int Row = 0; Row < Level->LevelGridData.LevelSize.x; ++Row) 
    {
        for(int Column = 0; Column < Level->LevelGridData.LevelSize.y; ++Column) 
        {
            if(Level->LevelGridData.LevelGrid[Row][Column] == ',') 
            {
                ++CommaOffset;
            }

            int TileX = (Column - CommaOffset) * TILESIZE;
            int TileY = (Row + 1) * TILESIZE;
            switch(Level->LevelGridData.LevelGrid[Row][Column]) 
            {
                case '0': 
                {
                }break;
                case '1': 
                {
                    DrawRectangle
                    (
                        TileX, 
                        TileY,
                        TILESIZE,
                        TILESIZE,
                        RED
                    );
                }break;
            }
        }
        CommaOffset = 0;
    }

    Color TestColor = {255, 255, 255, 100};

    DrawTexturePro
    (
        Level->LevelImage,
        Level->SrcRect,
        Level->DstRect,
        {0, 0},
        0,
        TestColor
    );
}

// FIXME : Terrible.

internal State 
InitGameState(State *State) 
{
    State->Textures = {};
    State->TextureCount = 2;
    State->Textures = (Texture2D *)malloc(int(sizeof(Texture2D)) * State->TextureCount);
    State->Textures[0] = 
        LoadTexture("../data/res/textures/PlayerCharacter.png");
    State->Textures[1] = 
        LoadTexture("../data/res/mapdata/Test/simplified/AutoLayers_advanced_demo/_composite.png");
    State->CurrentLevel = CreateLevel(State);

    State->Player.Pos = {0, 0};
    State->Player.Vel = {0, 0};
    State->Player.Rotation = 0;
    State->Player.Scale = 0;
    State->Player.MovementSpeed = 100.0f;
    State->Player.SpriteFrames = 9;

    State->Player.Sprite = State->Textures[0]; 
    State->Player.EntityType = PLAYER;
    State->Player.AnimationState = IDLE;
    State->Player.Flags.Flipped = false;
    State->Player.Flags.IsMoving = false;

    State->Player.IdleAnimation = CreateAnimation(0, 1, 0, 100, 96);
    State->Player.WalkingAnimation = CreateAnimation(4, 8, 4, 20, 24);
    
    return(*State);
}

int main() 
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello, Window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(144);

    State State;    
    InitGameState(&State);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        BeginMode2D(State.Camera);
        ClearBackground(BLUE); 

        HandleCamera(&State);
        HandlePlayerInput(&State);
        HandleAnimationStateMachine((Entity *)(&State.Player));
        DrawMap(&State.CurrentLevel);
        DrawFPS(0, 0);

        // TODO : Move this somewhere else, Just don't want it drawing every frame, my poor gpu would die.
        DrawMap(&State.CurrentLevel);

        DrawTexturePro
        (
            State.Player.Sprite, 
            State.Player.SrcRect, 
            State.Player.DstRect, 
            {State.Player.DstRect.width / 2, State.Player.DstRect.height}, 
            State.Player.Rotation, 
            WHITE
        );

#ifdef DEBUG
        DrawRectangleV
        (
            State.Player.Pos, 
            {real32(State.Player.Sprite.width / State.Player.SpriteFrames), real32(State.Player.Sprite.height)}, 
            RED
        );
#endif
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
    return(0);
}
