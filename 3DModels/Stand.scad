StandDepth = 35;
StandHeight = 25;
StandWidth = 80;
StandBorder=1.5;

StripeWidth = 7;
StripeBorder = 1.5;
StripeOuterWidth = StripeWidth+2*StripeBorder;
StripeBottom = 2;
StripeHeight = 14;
CableHeight = 2;

ConnectorGap = 20;
StandConnectorHeight = 10;
StandConnectorWidth = 44;
StandConnectorInnerWidth = StandConnectorWidth - 2*5;

Depth = StripeBottom + StripeHeight + CableHeight;

//Connector();
Stand();

module Stand()
{
    difference()
    {
        cube([StandWidth, StandDepth, StandHeight]);
        translate([0, StandBorder, StandBorder])
        {
            cube([StandWidth, StandDepth, StandHeight-StandBorder]);
        }
        
        translate([(StandWidth-StandConnectorInnerWidth)/2, 0, 0])
            cube([StandConnectorInnerWidth, Depth, StandConnectorHeight]);
        translate([(StandWidth-StandConnectorWidth)/2, 0, 0])
            cube([StandConnectorWidth, StripeBottom, StandConnectorHeight]);
    }   
}

module Connector()
{
    cube([StandWidth, StandDepth-2*StandBorder, StandConnectorHeight]);
}
