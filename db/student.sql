
-- drop database if exists  `hw-mto10-u1752877`;
-- create database `hw-mto10-u1752877`;  

use `hw-mto10-u1752877`;
drop table if exists   `student`;
create table `student`(
    stu_no char(7) not null,
    stu_name char(8) not null,
    stu_password char(32) not null,
    stu_common_password char(32) not null,
    stu_enable char(1) not null,
    primary key(stu_no)
);

delimiter $$
drop procedure if exists get_student;
create procedure get_student(in in_stu_no char(7))
begin
    if in_stu_no is null then
        select * from student;
    else
        select * from student where stu_no = in_stu_no; 
    end if;
end$$
drop procedure if exists get_pw;
create procedure get_pw(in in_stu_no char(7))
begin
    select stu_password from student where stu_no = in_stu_no;
end$$

drop procedure if exists check_login_gtest;
create procedure check_login_gtest(in in_stu_no char(7), in in_stu_password char(32))
begin
    declare res int;
    select count(*) from student where stu_no = in_stu_no and stu_password = in_stu_password into @res;
    if @res = 0 then
        select 0;
    else 
        set @grouptime = (unix_timestamp(current_timestamp()) div 1800) * 1800;
        set @cnt = (select count(*) from gameinfo_gtest where grouptime = @grouptime and no = in_stu_no);
        select @cnt+1;
    end if;
end$$

drop procedure if exists check_login_stest;
create procedure check_login_stest(in in_stu_no char(7), in in_stu_password char(32))
begin
    select count(*) from student 
    where stu_no = in_stu_no 
    and stu_password = in_stu_password;
end$$

drop procedure if exists check_login;
create procedure check_login(in in_stu_no char(7), in in_stu_password char(32), in in_test char(1))
begin
    if in_test = 's' then
        call check_login_stest(in_stu_no, in_stu_password);
    elseif in_test = 'g' then 
        call check_login_gtest(in_stu_no, in_stu_password);
    end if;
end$$

drop procedure if exists insert_student;
create procedure insert_student(
    in in_stu_no char(7),
    in in_stu_name char(8),
    in in_stu_password char(32),
    in in_stu_common_password char(32),
    in in_stu_enable char(1)
)
begin
    insert into `student` values(in_stu_no, in_stu_name, 
                in_stu_password, in_stu_common_password, '1');
end$$
delimiter ;


-- call insert_student(1752877, "ºúÐù", md5("xZx3aUL2%kt#-9+&"), 12345678, 1);
-- call get_student(1752877);