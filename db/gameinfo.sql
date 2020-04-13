use `hw-mto10-u1752877`;
drop table if exists `gameinfo_stest`;
drop table if exists `gameinfo_gtest`;
# 学号，MAPID，行，列，分数，步数，合成值，时长，结果，写入数据库

create table `gameinfo_stest` (
    `index` int auto_increment,
    `no` char(7) not null,
    `mapid` int not null,
    `row` tinyint not null,
    `col` tinyint not null,
    `score` int not null,
    `step` smallint not null,
    `maxvalue` tinyint not null,
    `createtime` bigint not null,
    `completetime` bigint not null,
    `result` tinyint not null,
    primary key(`index`)
);

create table `gameinfo_gtest` (
    `index` int auto_increment,
    `no` char(7) not null,
    `mapid` int not null,
    `row` tinyint not null,
    `col` tinyint not null,
    `score` int not null,
    `step` smallint not null,
    `maxvalue` tinyint not null,
    `createtime` bigint not null,
    `completetime` bigint not null,
    `grouptime` int not null,
    `result` tinyint not null,
    primary key(`index`)
);



drop view if exists view_gameinfo_stest;
create view view_gameinfo_stest
as 
select
from_unixtime(cast(createtime/1000 as int)) as createtime,
no,
stu_name as name,
mapid,
row,
col, 
score,
step,
g.maxvalue,
(completetime - createtime) as gametime,
g.result
from gameinfo_stest g, student s
where g.no = s.stu_no;

drop view if exists view_gameinfo_gtest;
create view view_gameinfo_gtest
as 
select
from_unixtime(grouptime) as grouptime,
no,
stu_name as name,
mapid,
row,
col, 
score,
step,
g.maxvalue,
(completetime - createtime) as gametime,
g.result
from gameinfo_gtest g, student s
where g.no = s.stu_no;

drop view if exists view_gameinfo_gtest_sumscore;
create view view_gameinfo_gtest_sumscore
as
select
grouptime,
count(*),
no,
name,
sum(score/row/col) as sumscore
from view_gameinfo_gtest
group by no, grouptime
order by grouptime desc, no asc;


-- create index timeindex on `gameinfo`(createtime);
-- create index noindex on `gameinfo`(no);

drop procedure if exists get_gameinfo_stest;
drop procedure if exists insert_gameinfo_stest;
drop procedure if exists get_gameinfo_gtest;
drop procedure if exists insert_gameinfo_gtest;
drop procedure if exists get_sum_score;
delimiter $$ 
create procedure insert_gameinfo_stest(
    in in_no char(7),
    in in_mapid int,
    in in_row tinyint,
    in in_col tinyint,
    in in_score int,
    in in_step smallint,
    in in_maxvalue tinyint,
    in in_createtime bigint,
    in in_completetime bigint,
    in in_result tinyint
)
begin
    insert into `gameinfo_stest` values(null, in_no, in_mapid, in_row, in_col, in_score, in_step, in_maxvalue, in_createtime, in_completetime, in_result);
end $$

create procedure get_gameinfo_stest(
    in in_mapid int, 
    in in_row tinyint, 
    in in_col tinyint, 
    in in_limit int)
begin
    set @sqlcmd = concat("select * from view_gameinfo_stest");
    if in_mapid is not null then
        set @sqlcmd = concat(@sqlcmd, " where mapid=",in_mapid);
        if in_row is not null and in_col is not null then
            set @sqlcmd = concat(@sqlcmd, " and row=", in_row, " and col=", in_col);
        end if;
    end if;
    set @sqlcmd = concat(@sqlcmd, " order by createtime desc");
    if in_limit is not null then 
        set @sqlcmd = concat(@sqlcmd, " limit ", in_limit);
    else
        set @sqlcmd = concat(@sqlcmd, " limit 10 ");
    end if;
    prepare stmt from @sqlcmd;
    execute stmt;
end$$



create procedure insert_gameinfo_gtest(
    in in_no char(7),
    in in_mapid int,
    in in_row tinyint,
    in in_col tinyint,
    in in_score int,
    in in_step smallint,
    in in_maxvalue tinyint,
    in in_createtime bigint,
    in in_completetime bigint,
    in in_result tinyint
)
begin
    insert into `gameinfo_gtest` values(
        null, 
        in_no, 
        in_mapid, 
        in_row, 
        in_col, 
        in_score, 
        in_step, 
        in_maxvalue, 
        in_createtime, 
        in_completetime, 
        (in_createtime div 1800000)*1800,
        in_result);
end $$

create procedure get_gameinfo_gtest(
    in in_grouptime datetime,
    in in_no char(7))
begin
    set @sqlcmd = concat("select * from view_gameinfo_gtest");
    set @sqlcmd = concat(@sqlcmd, " where grouptime='", in_grouptime,"'");
    set @sqlcmd = concat(@sqlcmd, " and no=", in_no);
    set @sqlcmd = concat(@sqlcmd, " order by mapid asc;");
    /* select @sqlcmd; */
    prepare stmt from @sqlcmd;
    execute stmt;
end$$

create procedure get_sum_score(
    in in_grouptime char(24),
    in in_no char(7)
)
begin
    set @sqlcmd = concat("select * from view_gameinfo_gtest_sumscore");
    if in_grouptime is not null or in_no is not null then
        set @sqlcmd = concat(@sqlcmd, " where ");
    end if;
    if in_grouptime is not null and not in_grouptime = '' then 
        set @sqlcmd = concat(@sqlcmd, " grouptime='", in_grouptime,"'");
        if in_no is not null and not in_no ='' then 
            set @sqlcmd = concat(@sqlcmd, " and ");
        end if;
    end if;
    if in_no is not null then 
        set @sqlcmd = concat(@sqlcmd, " no=", in_no);
    end if;
    /* select @sqlcmd; */
    prepare stmt from @sqlcmd;
    execute stmt;
end$$


delimiter ;

-- /* call insert_gameinfo_stest(1752877, 123456, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 5432543, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 0, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 54613, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 8946513, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 1861, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 315, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 36513, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 69834, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 6934651, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 365, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 68345, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 8654, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 64635, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 365136, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 63546, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 698, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 3426, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 87647, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 5642, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_stest(1752877, 5642654, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 43, unix_timestamp(current_timestamp())*1000, 1);
-- */
-- call insert_gameinfo_gtest(1752877, 123456, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 5432543, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 0, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 54613, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 8946513, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 1861, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 315, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 36513, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 69834, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 6934651, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 365, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 68345, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 8654, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 64635, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 365136, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 63546, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 698, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 3426, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 87647, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 5642, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 1000, unix_timestamp(current_timestamp())*1000, 1);
-- call insert_gameinfo_gtest(1752877, 5642654, 5, 5, 1000,
--     100, 8, unix_timestamp(current_timestamp())*1000 - 43, unix_timestamp(current_timestamp())*1000, 1); 


-- /* select unix_timestamp("2020-04-10 01:00:00");
-- select * from view_gameinfo_gtest; */
-- call get_gameinfo_gtest("2020-04-10 02:30:00", 1752877);

-- /* select * from view_gameinfo_gtest_sumscore; */
-- call get_sum_score("2020-04-10 02:30:00", 1752877);