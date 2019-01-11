struct kinematicState
{
     D3DXVECTOR3 position;
     D3DXVECTOR3 velocity;
     D3DXVECTOR3 acceleration;
};

void PredictPosition(kinematicState *old, kinematicState *prediction, float elapsedSeconds)
{
     prediction->position = old->position + (old->velocity * elapsedSeconds) + (0.5 * old->acceleration * (elapsedSeconds * elapsedSeconds));`
}

kinematicState *BlendKinematicStateLinear(kinematicState *olStated, kinematicState *newState, float percentageToNew)
{
     //Explanation of percentateToNew:
     //A value of 0.0 will return the exact same state as "oldState",
     //A value of 1.0 will return the exact same state as "newState",
     //A value of 0.5 will return a state with data exactly in the middle of that of "old" and "new".
     //Its value should never be outside of [0, 1].

     kinematicState *final = new kinematicState();

     //Many other interpolation algorithms would create a smoother blend,
     //But this is just a linear interpolation to keep it simple.

     //Implementation of a different algorithm should be straightforward.
     //I suggest starting with Catmull-Rom splines.

     float percentageToOld = 1.0 - percentageToNew;

     final->position = (percentageToOld * oldState->position) + (percentageToNew * new-State>position);
     final->velocity = (percentageToOld * oldState->velocity) + (percentageToNew * newState->velocity);
     final->acceleration = (percentageToOld * oldState->acceleration) + (percentageToNew * newState->acceleration);

     return final;
}

