$fn = 100;

Height = 196;
Width = 100;
RadiusOuter = 20;

StripeWidth = 7;
StripeBorder = 1.5;
StripeOuterWidth = StripeWidth+2*StripeBorder;
StripeBottom = 2;
StripeHeight = 14;
CableHeight = 2;
TrennerHeigh = 165;
TrennerAbstandUnten = 10;
TrennerAbstandOben = 3;

StandConnectorHeight = 10;
StandConnectorWidth = 44;
StandConnectorInnerWidth = StandConnectorWidth - 2*5;

Depth = StripeBottom + StripeHeight + CableHeight;
InnerWidth = Width - StripeWidth;

ConnectorGap = 20;

StandDepth = 160;
StandHeight = 33;
StandWidth = 160;
StandBorder = 2;

ESP32Width = 28;
ESP32Length = 49;
ESP32XOffset = 30;
ESP32YOffset = 35;

ESP32PCBHolderDepth = 7;
ESP32PCBHolderShortDepth = 8;
ESP32PCBHolderHeight = 13;
Esp32PCBWidth = 48.8;
ESP32PCBLength = 71;
ESP32PCBShortLength = 10;
ESP32PCBOffsetX = ESP32XOffset-10.5;
ESP32PCBOffsetY = ESP32YOffset-ESP32PCBHolderDepth-10;
ESP32PCBOffsetZ = 9.5;
ESP32PCBThickness = 2;
ESP32PCBGapDepth = 2;

RFIDHolderOffsetY = ESP32YOffset+103;
PFIDHolderOffsetX = ESP32PCBOffsetX+3;
RFIDHolderHoleDistanceX = 25.5;
RFIDHolderHeight = 3;

RTCHolderOffset_X = 133;
RTCHolderOffsetY = ESP32PCBOffsetY-3;
RTCHolderHoleDistance = 19.5;

// ConnectorHolderOffsetZ = 11;
ConnectorPCBWidth = 20.3;
ConnectorPCBLength = 80;
ConnectorHolderBottomHeight = 1.2;
ConnectorholderBottomWidth = 10;
ConnectorHolderBottomDepth = 8;
ConnectorHolderPCBGap = 2;
ConnectorHolderPCBDepth = 2.0;
ConnectorHolderPCBHeight = 5.5;
ConnectorHolderPCBWidth = 3.5;

KeypadKeyBreite = 13;
KeypadSpalten = 5;
KeypadReihen = 4;
KeypadAussparungBreite = KeypadKeyBreite * KeypadSpalten;
KeypadAussparungTiefe = KeypadKeyBreite * KeypadReihen;
KeypadOffsetY = 145;
KeypadOffsetX = 75;
KeypadHalterHoehe = 3.5;
KeypadPlatinenBreite = 100;
KeypadHalterOffsetX1 = 17.5;
KeypadHalterOffsetX2 = 19.5;
KeypadHalterOffsetY1 = 6;
KeypadHalterOffsetY2 = 71;
KeypadSpalt = 0.7;

DisplayHalterDurchmesserBohrung = 3;
DisplayHalterOffsetX = -1.65;
DisplayHalterOffsetY = 7.4;
DisplayHalterKantenlaenge = DisplayHalterDurchmesserBohrung + 4;

Keys =       ["►", "6", "‖", "8", "©", "1", "2", "3", "4", "T3", "▼", "▲", "X", "H", "T2", "+", "-", "▐◄", "►▌", "T1"];
KeysOffset = [0,   0,   3,   0,   0,   0,   0,   0,   0,   -2,   0,   0,   0,   0,   -2,   0,   0,   -3.5, -3,   -2];
KeysSize   = [2,   0,   2,   0,   1,   0,   0,   0,   0,   1,    1,   1,   1,   1,    1,   1,   1,   1,    1,    1];

ScrewDiameter = 3;
DigitAbstand = 25;

SchwalbenschwanzHoehe = 5;
SchwalbenschwanzBreite1 = 5;
SchwalbenschwanzBreite2 = 3;

BottomBorderHeight = 8;

//Digit(DigitAbstand, false, true);
// translate([0, Width + DigitAbstand, 5])
//     Digit(0, false, false);    
// translate([0, 2*Width + 2*DigitAbstand, 0])
//     Trenner();

// translate([0, 2*Width + 3*DigitAbstand+StripeOuterWidth, 5])
//     Digit(DigitAbstand, false, false);

// translate([0, 3*Width + 4*DigitAbstand+StripeOuterWidth, 0])
//     Digit(0, true, false); 

//Stand();
//StandInvers();

Bottom();

module Bottom()
{
    cube([StandWidth, StandDepth, StandBorder]);
    translate([StandBorder, StandBorder, StandBorder])
    difference()
    {
        cube([StandWidth-2*StandBorder, StandDepth-2*StandBorder, BottomBorderHeight]);
        translate([StandBorder, StandBorder, 0])
        {
            cube([StandWidth-4*StandBorder, StandDepth-4*StandBorder, BottomBorderHeight+1]);
        }
    }
}

//Key("A", true, 0, 2);
module StandInvers()
{
    translate([KeypadOffsetX, KeypadOffsetY, StandBorder])
    {
        rotate([180,0,0])
            KeyPad(false);
    }
    
    translate([23, 90, 0])
        scale([0.1, 0.1, 1])
            linear_extrude(StandBorder)
                import("rfid.svg");
}

module Stand()
{
    difference()
    {
        cube([StandWidth, StandDepth, StandHeight]);
        translate([StandBorder, StandBorder, StandBorder])
        {
            cube([StandWidth-2*StandBorder, StandDepth-2*StandBorder, StandHeight]);
        }
        
        // ESP32 Hole
        translate([ESP32XOffset, ESP32YOffset, 0])
            cube([ESP32Length, ESP32Width, 100]);
        
        translate([KeypadOffsetX, KeypadOffsetY, StandBorder])
        {
            rotate([180,0,0])
                KeyPad(true);
        }
        
        translate([23, 90, 0])
            scale([0.1, 0.1, 1])
                linear_extrude(10)
                    import("rfid.svg");

                
        translate([(StandWidth-StandConnectorInnerWidth)/2, 0, 0])
            cube([StandConnectorInnerWidth, Depth, StandConnectorHeight]);
        translate([(StandWidth-StandConnectorWidth)/2, 0, 0])
            cube([StandConnectorWidth, StripeBottom, StandConnectorHeight]);
    }
    translate([KeypadOffsetX-KeypadAussparungTiefe/2, KeypadOffsetY-KeypadAussparungBreite, StandBorder])
        KeypadHalter();
    
    // ESP32 Holder
    translate([ESP32PCBOffsetX, ESP32PCBOffsetY+ESP32PCBGapDepth, StandBorder])
        ESP32PCBHolderLong();  
    translate([ESP32PCBOffsetX, ESP32PCBOffsetY+ESP32PCBHolderDepth+Esp32PCBWidth-ESP32PCBGapDepth, StandBorder])
    {
        ScrewHolder(ESP32PCBOffsetZ, 0);
    }
    translate([ESP32PCBOffsetX+ESP32PCBLength-ESP32PCBShortLength, ESP32PCBOffsetY+ESP32PCBHolderDepth+Esp32PCBWidth-ESP32PCBGapDepth, StandBorder])
        ScrewHolder(ESP32PCBOffsetZ, 0);
    
    // RFID Holder
    translate([PFIDHolderOffsetX, RFIDHolderOffsetY-ESP32PCBHolderShortDepth, StandBorder])
    {
        ScrewHolder(RFIDHolderHeight, 0);
    }
    translate([PFIDHolderOffsetX+RFIDHolderHoleDistanceX, RFIDHolderOffsetY-ESP32PCBHolderShortDepth, StandBorder])
        ScrewHolder(RFIDHolderHeight, 0);
    
    // ConnectorHolder
    translate([StandWidth,5,StandBorder])
    rotate([0,0,90])
        ConnectorHolder();
    
    // RTC Holder
   
}

module ConnectorHolder()
{
    cube([ConnectorholderBottomWidth, ConnectorHolderBottomDepth, ConnectorHolderBottomHeight]);
    translate([0, 0, ConnectorHolderBottomHeight])
    {
        cube([ConnectorholderBottomWidth, ConnectorHolderPCBDepth, ConnectorHolderPCBGap]);
        translate([ConnectorholderBottomWidth, 0, 0])
            rotate([0,-90,0])
                linear_extrude(ConnectorholderBottomWidth)
                    polygon(points=[[ConnectorHolderPCBHeight,0],[ConnectorHolderPCBGap,0],[ConnectorHolderPCBGap,ConnectorHolderPCBWidth],]);
    }

    translate([ConnectorPCBLength-ConnectorholderBottomWidth*1.5, 0, 0])
    {
        cube([ConnectorholderBottomWidth, ConnectorHolderBottomDepth, ConnectorHolderBottomHeight]);
        translate([0, 0, ConnectorHolderBottomHeight])
        {
            cube([ConnectorholderBottomWidth, ConnectorHolderPCBDepth, ConnectorHolderPCBGap]);
            translate([ConnectorholderBottomWidth, 0, 0])
                rotate([0,-90,0])
                    linear_extrude(ConnectorholderBottomWidth)
                        polygon(points=[[ConnectorHolderPCBHeight,0],[ConnectorHolderPCBGap,0],[ConnectorHolderPCBGap,ConnectorHolderPCBWidth],]);
        }
    }

    translate([ConnectorPCBLength-ConnectorholderBottomWidth*.5, 2*ConnectorHolderPCBDepth+ConnectorPCBWidth+1, 0])
    {
        rotate([0,0,180])
        {
            cube([ConnectorholderBottomWidth, ConnectorHolderBottomDepth, ConnectorHolderBottomHeight]);
            translate([0, 0, ConnectorHolderBottomHeight])
            {
                cube([ConnectorholderBottomWidth, ConnectorHolderPCBDepth, ConnectorHolderPCBGap]);
                translate([ConnectorholderBottomWidth, 0, 0])
                    rotate([0,-90,0])
                        linear_extrude(ConnectorholderBottomWidth)
                            polygon(points=[[ConnectorHolderPCBHeight,0],[ConnectorHolderPCBGap,0],[ConnectorHolderPCBGap,ConnectorHolderPCBWidth],]);
            }

            translate([ConnectorPCBLength-ConnectorholderBottomWidth*1.5, 0, 0])
            {
                cube([ConnectorholderBottomWidth, ConnectorHolderBottomDepth, ConnectorHolderBottomHeight]);
                translate([0, 0, ConnectorHolderBottomHeight])
                {
                    cube([ConnectorholderBottomWidth, ConnectorHolderPCBDepth, ConnectorHolderPCBGap]);
                    translate([ConnectorholderBottomWidth, 0, 0])
                        rotate([0,-90,0])
                            linear_extrude(ConnectorholderBottomWidth)
                                polygon(points=[[ConnectorHolderPCBHeight,0],[ConnectorHolderPCBGap,0],[ConnectorHolderPCBGap,ConnectorHolderPCBWidth],]);
                }
            }
        }
    }
}

module ESP32PCBHolderLong()
{
    difference()
    {
        cube([ESP32PCBLength, ESP32PCBHolderDepth, ESP32PCBHolderHeight]);
        translate([0,ESP32PCBHolderDepth-ESP32PCBGapDepth,ESP32PCBOffsetZ])
            cube([ESP32PCBLength, ESP32PCBGapDepth, ESP32PCBThickness]);
    }
}

module ScrewHolder(height, rotate)
{
    rotate([0,0,rotate])
    {
        difference()
        {
            cube([ESP32PCBShortLength, ESP32PCBHolderShortDepth, height]);
            translate([ESP32PCBShortLength/2, ESP32PCBHolderShortDepth-ScrewDiameter, 0])
                cylinder(d=ScrewDiameter, h=100);
        }
    }
}

module KeyPad(invers)
{
    if (invers)
    {
        translate([0,KeypadAussparungTiefe,0])
            cube([KeypadAussparungBreite + KeypadSpalt,KeypadSpalt,StandBorder]);
    }

    for (y=[0:KeypadReihen-1])
    {
        for (x=[0:KeypadSpalten-1])
        {
            index = y*KeypadSpalten+x;
            translate([x*KeypadKeyBreite,y*KeypadKeyBreite,0])
            {
                Key(Keys[index], invers, KeysOffset[index], KeysSize[index]);
            }
        }
    }
}

module Key(text, invers, offset, keySize)
{
    if (keySize > 0)
    {
        if (invers)
        {
            difference()
            {
                cube([KeypadKeyBreite*keySize + KeypadSpalt,KeypadKeyBreite*keySize-2,StandBorder+1.5]);
                translate([KeypadSpalt,KeypadSpalt,StandBorder-1])
                {
                    cube([KeypadKeyBreite*keySize-KeypadSpalt,KeypadKeyBreite*keySize,StandBorder]);
                }
            }
        }
        
        translate([4.5+offset,4,1])
        {
            linear_extrude(1)
            { 
                if (keySize == 1)
                {
                    size = 6 - len(text) * 0.5;
                    text(text, size=size, font = "Arial:style=Bold");
                }
                else
                {
                    size = (6 - len(text) * 0.5)*3;
                    text(text, size=size, font = "Arial:style=Bold");
                }
                
            }
        }
    }
}

module KeypadHalter()
{
  Rechts = KeypadHalterOffsetX1;
  Links = KeypadHalterOffsetX1 + 83.8;
  Links_alt = 0 - KeypadHalterOffsetX2;
  Unten = KeypadHalterOffsetY1;
  Oben = KeypadHalterOffsetY1 + 63.5;
  Oben_alt = KeypadHalterOffsetY2;
    
  translate([Rechts,Unten,0])
  {
    Halter(DisplayHalterKantenlaenge, KeypadHalterHoehe, DisplayHalterDurchmesserBohrung);
  }
  
  translate([Links,Unten,0])
  {
    Halter(DisplayHalterKantenlaenge, KeypadHalterHoehe, DisplayHalterDurchmesserBohrung);
  }
   
  translate([Rechts,Oben,0])
  {
    Halter(DisplayHalterKantenlaenge, KeypadHalterHoehe, DisplayHalterDurchmesserBohrung);
  }
  
  translate([Links,Oben,0])
  {
    Halter(DisplayHalterKantenlaenge, KeypadHalterHoehe, DisplayHalterDurchmesserBohrung);
  }
}

module Halter(Kantenlaenge, Hoehe, DurchmesserBohrung)
{
  translate([-Kantenlaenge/2, -Kantenlaenge/2, 0])
  {
    difference()
    {
      cube([Kantenlaenge, Kantenlaenge, Hoehe]);
      translate([Kantenlaenge/2, Kantenlaenge/2,0])
      {
        cylinder(h=Hoehe, d=DurchmesserBohrung);       
      }
    }
  }
}

module Trenner()
{
    difference()
    {
        union()
        {
            translate([0,-DigitAbstand,0])
                DigitConnector(StripeOuterWidth+DigitAbstand*3,0, false);
            translate([-StandConnectorHeight, -Width/2+StripeOuterWidth/2,0])
                StandConnector();

            cube([TrennerHeigh+TrennerAbstandOben+TrennerAbstandUnten, StripeOuterWidth, Depth]);

            translate([-StandConnectorHeight/2,(StripeOuterWidth+DigitAbstand*3)/2+5,0])
                SchwalbenschwanzVerbinder();
        }

        // Aussparung Mitte
        translate([-StandConnectorHeight-2*StripeBorder-50,-ConnectorGap/2+StripeOuterWidth/2,-0.001-StripeBorder])
            cube([StandConnectorHeight+StripeBorder+50, ConnectorGap, Depth]);

        // Stripe Aussparung
        translate([StandConnectorHeight,StripeBorder,StripeBottom+CableHeight])
            cube([TrennerHeigh, StripeWidth, Depth*2]);
        translate([StandConnectorHeight-200,StripeBorder+1,StripeBottom])
            cube([TrennerHeigh+200, StripeWidth-2, Depth-2*StripeBottom]);

        // Horizontaler Kabelsteg
        translate([-StandBorder-2,-100,-0.1])
            cube([2, 200, 8]);

        translate([-StandConnectorHeight/2,(-StripeOuterWidth-DigitAbstand*3)/2+SchwalbenschwanzHoehe,0])
            SchwalbenschwanzVerbinder();
    }
}

module SchwalbenschwanzVerbinder()
{
    linear_extrude(Depth)
    {
        polygon(points=[
            [SchwalbenschwanzBreite2/2,0],
            [SchwalbenschwanzBreite1/2,SchwalbenschwanzHoehe],
            [-SchwalbenschwanzBreite1/2,SchwalbenschwanzHoehe],
            [-SchwalbenschwanzBreite2/2,0]]);
    }
}

module Digit(connectorOffsetY, endeLinks, endeRechts)
{
    difference()
    {
        union()
        {
            DigitConnector(Width,connectorOffsetY, true, endeLinks, endeRechts);
            DigitHolder(Width);
        }
        
        // translate([-StandConnectorHeight,(Width-ConnectorGap)/2,-StripeBorder])
        //     cube([StandConnectorHeight+StripeBorder, ConnectorGap, Depth]);
        // translate([StripeBorder,(Width-ConnectorGap)/2,StripeBottom+CableHeight])
        //     cube([StandConnectorHeight, ConnectorGap, Depth]);
        // translate([StripeBorder,(Width-ConnectorGap)/2,StripeBottom])
        //     cube([3, ConnectorGap, Depth]);
        
        // translate([-StripeBottom,Width-5-(Width-StandConnectorWidth)/2,-StripeBottom])
        //     cube([StripeBottom, 5, Depth]);
        // translate([-StripeBottom,(Width-StandConnectorWidth)/2,-StripeBottom])
        //     cube([StripeBottom, 5, Depth]);
    }
}


module DigitHolder(width)
{
    difference()
    {
        union()
        {
            Base(width);
            translate([(Height-StripeOuterWidth)/2, 0, 0])
                cube([StripeOuterWidth, Width, Depth]);
        }
        
        translate([0,0,StripeBottom + CableHeight])  
        {
            difference()
            {
                RoundedRectangle(RadiusOuter, StripeBorder);
                RoundedRectangle(RadiusOuter, StripeBorder + StripeWidth);
            }
        }
        
        translate([0,0,StripeBottom])  
        {
            difference()
            {
                RoundedRectangle(RadiusOuter, StripeBorder+1);
                RoundedRectangle(RadiusOuter, StripeBorder+StripeWidth-1);
            }
        }
        
        translate([(Height-StripeOuterWidth)/2+StripeBorder, StripeWidth/2, StripeBottom+CableHeight])
           cube([StripeWidth, InnerWidth, Depth]);
        
        translate([(Height-StripeOuterWidth)/2+StripeBorder+2, StripeWidth/2, StripeBottom])
           cube([StripeWidth-4, InnerWidth, Depth]);
    }
}

module Base(width)
{
    difference()
    {
        RoundedRectangle(RadiusOuter, 0);
        RoundedRectangle(RadiusOuter, StripeOuterWidth);
        
        // Durchbruch
        translate([-StandBorder-2, width/2,3])
            cube([10, 20, 8]);
    }
}

module RoundedRectangle(radius, offset)
{
    offsetradius = radius - offset;
    hull()
    {
        translate([radius, radius, 0])
            cylinder(h=Depth, r=offsetradius);
        translate([radius, Width-radius, 0])
            cylinder(h=Depth, r=offsetradius);
        translate([Height-radius, Width-radius, 0])
            cylinder(h=Depth, r=offsetradius);
        translate([Height-radius, radius, 0])
            cylinder(h=Depth, r=offsetradius);
    }
}

module DigitConnector(width, offsetY, durchbruch, endeLinks, endeRechts)
{
    difference()
    {
        union()
        {
            translate([-StandConnectorHeight,-DigitAbstand/2+offsetY,0])
            {
                cube([StandConnectorHeight, width, Depth]);
                if (!endeLinks)
                {
                    translate([StandConnectorHeight/2,width,0])
                        SchwalbenschwanzVerbinder();
                }
            }
        }

        // Durchbruch
        if (durchbruch)
            translate([-StandBorder-2, width/2,3])
                cube([20, 20, 8]);

        if (!endeRechts)
        {
            translate([-StandConnectorHeight/2,-DigitAbstand/2+offsetY,0])
                SchwalbenschwanzVerbinder();
            // Horizontaler Kabelsteg
            translate([-StandBorder-2,-325,-0.1])
                cube([2, 400, 8]);
        }
        if (!endeLinks)
        {
            // Horizontaler Kabelsteg
            translate([-StandBorder-2,25,-0.1])
                cube([2, 400, 8]);
        }
        if (!endeLinks && !endeRechts)
        {
            // Horizontaler Kabelsteg
            translate([-StandBorder-2,-200,-0.1])
                cube([2, 400, 8]);
        }
    }
}

module StandConnector()
{
    difference()
    {
        translate([-StandConnectorHeight,(Width-StandConnectorWidth)/2,0])
            cube([StandConnectorHeight, StandConnectorWidth, Depth]);

        translate([-StripeBottom,Width-5-(Width-StandConnectorWidth)/2,-StripeBottom])
            cube([StripeBottom, 5, Depth]);
        translate([-StripeBottom,(Width-StandConnectorWidth)/2,-StripeBottom])
            cube([StripeBottom, 5, Depth]);
    }
}




