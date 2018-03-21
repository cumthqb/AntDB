--
-- regress test minus for  grammar=oracle
-- add feature http://192.168.11.45:8000/issues/25263
--
set grammar to oracle ;

-- create test table
-- scene 1:表结构相同，全表进行minus
create table tab_minus_1 (id int, name varchar(22),age int);
create table tab_minus_2 (id int, name varchar(22),age int);
insert into tab_minus_1 values(1,'jay1',20);
insert into tab_minus_1 values(2,'jay2',21);
insert into tab_minus_1 values(3,'jay3',22);
insert into tab_minus_1 values(4,'jay4',23);
insert into tab_minus_1 values(5,'jay5',24);

insert into tab_minus_2 values(1,'jay1',20);
insert into tab_minus_2 values(2,'jay2',21);
insert into tab_minus_2 values(3,'may3',22);
insert into tab_minus_2 values(6,'jay6',26);

select * from tab_minus_1 
minus
select * from tab_minus_2
order by 3;

--scene 2: 表结构不相同，全表进行minus
create table tab_minus_3 (id int,name varchar(22));
insert into tab_minus_3 values(1,'jay1');
insert into tab_minus_3 values (2,'jay2');
insert into tab_minus_3 values (3,'may3');
insert into tab_minus_3 values (8,'may8');

select * from tab_minus_1 
minus
select * from tab_minus_3;   --should be failed

-- scene 3:表结构相同，部分字段进行minus
select id from tab_minus_1
minus
select id from tab_minus_2
order by 1;   --should be sucessed
select id,age from tab_minus_1 
minus
select id,age from tab_minus_2
order by 2; --should be sucessed
select id,age from tab_minus_1
minus
select id,name,age from tab_minus_2;   --should be failed
select id,name,age from tab_minus_1
minus
select id,age from tab_minus_2;   --should be failed

-- scene 4:表结构不相同，部分字段进行minus
select id from tab_minus_1
minus
select id from tab_minus_3
order by 1; --should be sucessed
select id,name from tab_minus_1
minus
select id,name from tab_minus_3
order by 2;   --should be sucessed
select * from tab_minus_1
minus
select id,name from tab_minus_3; --should be failed

-- scene 5:minus用在where 条件中进行筛选
select * from  tab_minus_1 a 
where 
a.id in(select id from tab_minus_2
        minus
        select id from tab_minus_3) order by a.id desc; --should be sucessed



-- init table/data 
drop table tab_minus_1;
drop table tab_minus_2;
drop table tab_minus_3;  
