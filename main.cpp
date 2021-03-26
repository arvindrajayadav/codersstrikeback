#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//basic vector for easier organization
struct Vec2
{
    int x,y;
    Vec2(int X = 0, int Y = 0) : x(X), y(Y) {}
};

Vec2 operator+(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x+b.x, a.y+b.y);
}
Vec2 operator-(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x-b.x, a.y-b.y);
}
Vec2 operator*(float c, const Vec2& a)
{
    return Vec2(c*a.x, c*a.y);
}
bool operator==(const Vec2& a, const Vec2& b)
{
    return a.x == b.x && a.y == b.y;
}
float distanceSquared(const Vec2& a, const Vec2& b)
{
    return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

//functions to hide the cout statements and make the code more readable

void MoveTo(int x, int y, int thrust)
{
    cout << x << " " << y << " " << thrust << endl;
}

void BoostTo(int x, int y)
{
    cout << x << " " << y << " BOOST"<< endl;
}

int main()
{
    //maximum speed we are allowed
    const int MAX_THRUST = 100;

    //deciding when to use boost
    //we should at least be this far away from our next checkpoint
    const int MIN_BOOST_DISTANCE = 800;

    //when using boost, we should also be facing our checkpoint
    //otherwise you might boost in a different direction
    //this is the maximum angle difference to use boost
    const int MAX_BOOST_ANGLE = 10;

    //the checkpoint radius is specified in the problem description
    //since sqrt is an expensive operation, we'll compare the square of our distances where possible
    const float CHECKPOINT_RADIUS_SQUARED = 360000.f;

    //this is the square of distance from the next checkpoint where we start slowing down
    const float SLOWDOWN_RADIUS_SQUARED = 1000000.0f;

    //when do we want to start taking angle to next checkpoint into account for slowdowns
    const int SLOWDOWN_ANGLE_THRESHOLD = 15;

    //have we used our boost already? (once per game)
    bool usedBoost = false;

    //our position in the last frame
    Vec2 prevPos;

    // game loop
    while (1) {
        int x;
        int y;
        int nextCheckpointX; // x position of the next check point
        int nextCheckpointY; // y position of the next check point
        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> nextCheckpointX >> nextCheckpointY >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        int opponentX;
        int opponentY;
        cin >> opponentX >> opponentY; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        //the final thrust value
        int finalThrust;
        
        //thrust needs to vary depending on our angle to avoid long turns
        //we should also be correcting our angle actively if possible

        //magnitude of the angle
        int abs_nextCheckpointAngle = abs(nextCheckpointAngle);

        //are we directly facing the next checkpoint?
        if(abs_nextCheckpointAngle == 0)
        {
            //we are directly facing the next checkpoint
            //this is the ideal time for boost, but only if we're far away enough
            if(!usedBoost && nextCheckpointDist > MIN_BOOST_DISTANCE)
            {
                BoostTo(nextCheckpointX, nextCheckpointY);
                usedBoost = true;    
            }
            else
                MoveTo(nextCheckpointX, nextCheckpointY, MAX_THRUST);
        }
        else
        {
            //we are not directly facing the next checkpoint
            
            const Vec2 curPos = {x, y};
            const Vec2 deltaPos = curPos - prevPos;
            
            //our destination is the next checkpoint
            Vec2 destination = {nextCheckpointX, nextCheckpointY};

            const float distToDestinationSquared = distanceSquared(curPos, destination);

             //our final speed
            int thrust = MAX_THRUST;

            //if we are within a certain distance of the checkpoint, we should start slowing down
            if(distToDestinationSquared < SLOWDOWN_RADIUS_SQUARED)
            {
                //slowdown amount inversely proportional to distance from checkpoint
                const float speedMod_Distance = clamp(distToDestinationSquared / (CHECKPOINT_RADIUS_SQUARED * 4.f), 0.1f, 1.f);
                thrust *= speedMod_Distance;

                 cerr << "speedMod_Distance " << speedMod_Distance << endl;
            }
            
            //if we're not directly looking at the checkpoint, we should slow down
            //how much we slow down is proportional to how far away we're looking from the checkpoint
            if(abs_nextCheckpointAngle > SLOWDOWN_ANGLE_THRESHOLD)
            {
                //the more the angle, the more the slowdown
                const float speedMod_Angle = 1.f - clamp(abs_nextCheckpointAngle / 90.f, 0.f, 1.f);
                thrust *= speedMod_Angle;

                 cerr << "speedMod_Angle " << speedMod_Angle << endl;
            }

            //aim a little ahead of the checkpoint
            //destination = destination - (3.f * deltaPos);

            MoveTo(destination.x, destination.y, thrust);
        }

        //end of loop

        //update previous postion
        prevPos.x = x;
        prevPos.y = y;     
    }
}