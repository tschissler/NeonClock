$fn=200;

BaseDiameter = 31;
BaseHeight = 6;
TagDiameter = 26;
TagHeight = 0.9;
FullHeight = 6;
HandleThickness = 5;
WallThickness = 1.6;

CaseCapHeigh = 10;
CaseHeight = 14 * FullHeight + 2;

invers = true;
//Tag();
//Case();
CaseCap();

module Tag()
{
    difference()
    {
        if (invers)
            FullBase();
        translate([0,0,BaseHeight-1])
            Text("3");
    }
}

module Case()
{
    difference()
    {
        cylinder(h=CaseHeight+WallThickness, d=BaseDiameter+1+2*WallThickness);
        translate([0,0,WallThickness])
            cylinder(h=CaseHeight, d=BaseDiameter+1);
    }
}

module CaseCap()
{
    difference()
    {
        cylinder(h=CaseCapHeigh+WallThickness, d=BaseDiameter+1+4*WallThickness);
        translate([0,0,WallThickness])
            cylinder(h=CaseCapHeigh, d=BaseDiameter+1+2*WallThickness);
    }
}

module FullBase()
{
    difference()
    {
        cylinder(h=FullHeight, d=BaseDiameter);
        cylinder(h=TagHeight, d=TagDiameter);
    }
}

module Text(text)
{
    textsize = textmetrics(text, size=15, font="Arial:style=Bold").size;
    textposition = textmetrics(text, size=15, font="Arial:style=Bold").position;

    translate([-(textsize.x + textposition.x)/2,-(textsize.y + textposition.y)/2,0])
    {
        linear_extrude(height=1)
        {
            text(text, size=15, font="Arial:style=Bold", $fn=200);
        }
        // translate([textposition.x, textposition.y,1])
        //     cube([textsize.x, textsize.y, 1]);
    }
}