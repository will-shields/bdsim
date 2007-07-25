# ALL LENGTHS MUST BE GIVEN IN MILLIMETRES
# ALL ROTATIONS MUST BE GIVEN IN RADIANS



DROP DATABASE IF EXISTS LEAD_PLATE;
CREATE DATABASE LEAD_PLATE;
USE LEAD_PLATE;








CREATE TABLE plate_box (
		PARENTNAME		VARCHAR(32),
    PosX          DOUBLE(10,3), #
    PosY          DOUBLE(10,3), #
    PosZ          DOUBLE(10,3), #
    Red           DOUBLE(10,3), #
    Blue          DOUBLE(10,3), #
    Green         DOUBLE(10,3), #
    VisAtt        VARCHAR(32),  # i = invis, s = solid, w = wireframe
    LengthX       DOUBLE(10,3), #
    LengthY       DOUBLE(10,3), #
    LengthZ       DOUBLE(10,3), #
    Material      VARCHAR(32),  # material, CGA literal name
    Name          VARCHAR(32)   # name of solid, logical, and physical volume
);

                         

#INSERT INTO plate_box VALUES ("",-346.1, 0.0, 1, 1.0, 0.0, 0.0, "s", 103.0, 
#103.0, 3.0, "Lead", "lead_plate");


INSERT INTO plate_box VALUES ("",65.0, 0.0, 27, 0.0, 1.0, 0.0, "s", 100.0, 
100.0, 54.0, "Lead", "lead_plate");


