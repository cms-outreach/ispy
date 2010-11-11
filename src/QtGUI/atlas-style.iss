/*  Lucas preferred style.
    
    Colour table at:
    
    http://www.december.com/html/spec/colorsafecodes.html
 */


// Default style.
* {
  diffuse-color: rgb(0.7, 0.7, 0.7);
  transparency: 0.0;
  line-width: 1.0;
  line-pattern: 0xffff;
  font-size: 12;
  font-family: Arial;
  draw-style: solid;
  marker-shape: square;
  marker-size: normal;
  marker-style: filled;
  text-align: left;
  min-energy: 0.2;
  max-energy: 5.0;
  energy-scale: 1.0;
  left: 0.;
  top: 0.;
}

Background {
  diffuse-color: rgb(0, 0, 0);
}

Event_V2 {
  diffuse-color: rgb(1.0, 1.0, 1.0); 
  font-size: 24.0; 
  font-family: Arial;
  background: :/images/atlas-small.png;
  left: 0.125;
  top: 0.025;
  annotation-level: press;
}

ATLASTracks_V1
{
  draw-style: lines; line-width: 3;
  diffuse-color: rgb(1.0, 0.7, 0.1);
  marker-style: outline;
}

