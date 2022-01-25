create table employees(
   e_id CHAR(4) PRIMARY KEY,
   e_name VARCHAR(20),
   address VARCHAR(50),
   email VARCHAR(60),
   age VARCHAR(2),
   contact_num CHAR(10));

create table doctor( e_id CHAR(4) PRIMARY KEY REFERENCES employees,
                    specializes_in VARCHAR(30));

create table helping_staff( e_id CHAR(4) PRIMARY KEY REFERENCES employees);

create table patients ( p_id CHAR(3) PRIMARY KEY,
                        p_name VARCHAR(20),
                        referring_doctor CHAR(4) REFERENCES doctor,
                        age CHAR(2));
create table rooms( r_no CHAR(3) PRIMARY KEY,
                     floor CHAR(2),
                     e_id CHAR(4) REFERENCES helping_staff);
