// William A Stevens V
// Reflective prism base clip for 1/16 inch lexan with a 1.5 inch bottom side length.
s=1.5*25.4;
for(i = [0:4]) {
    rotate([0,0,90*i]) translate([-(s/2),-(s/2)-2.5,0]) union (){
        translate([0,3,0]) cube([5,6,5.5]);
        difference() {
            translate([0,-3,0]) cube([s,7,5.5]);
            rotate([90-54.735,0,0]) translate([0,1,1]) cube([s,1.75,6]);
        }
    }
}