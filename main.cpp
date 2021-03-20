#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    //maximum speed we are allowed
    const int MAX_THRUST = 100;

    //deciding when to use boost
    //we should at least be this far away from our next checkpoint
    const int MIN_BOOST_DISTANCE = 1200;

    //when using boost, we should also be facing our checkpoint
    //otherwise you might boost in a different direction
    //this is the maximum angle difference to use boost
    const int MAX_BOOST_ANGLE = 10;

    //have we used our boost already? (once per game)
    bool usedBoost = false;

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
        
        //don't accelerate when we're facing away from the next checkpoint
        //this helps us turn faster
        if (nextCheckpointAngle < -90 || 
            nextCheckpointAngle > 90) 
            finalThrust = 0;
        else 
            finalThrust = MAX_THRUST;

        if(!usedBoost)
        {
            //if we're facing our target and it's far away, boost
            if(nextCheckpointDist > MIN_BOOST_DISTANCE && nextCheckpointAngle < MAX_BOOST_ANGLE)
            {
                cout << nextCheckpointX << " " << nextCheckpointY << " BOOST"<< endl;
                usedBoost = true;
            }    
        }
        else
        {
            //normal movement

            // You have to output the target position
            // followed by the power (0 <= thrust <= 100)
            // i.e.: "x y thrust"
            cout << nextCheckpointX << " " << nextCheckpointY << " " << finalThrust << endl;
        }        
    }
}