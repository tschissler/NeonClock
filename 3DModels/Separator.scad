$fn=50;

Radius = 4;
Height = 16;
Height2 = Height-Radius;
Width = 6.5;
Thickness = 10;
GapHeight = 10.5;
GapWidth = 2.8;

difference()
{
    union()
    {
        cylinder(h=Thickness, r=Radius);
        translate([-Width/2, 0, 0])
            cube([Width, Height2, Thickness]);
    }
        
    translate([(Width-GapWidth)/2,Height2-GapHeight,0])
    {
        cube([GapWidth, GapHeight, Thickness]);
    }
}

translate([-Radius,90, 0])
    rotate([0, -70, 180])
        cube([100,100,100]);