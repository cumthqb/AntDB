-- 
-- in/not in  for grammar = oracle 
-- Solution bug: http://192.168.11.45:8000/issues/25268
--
set grammar to oracle;

-- create test table
create table tab_in (id int ,name varchar(22),age numeric(10,2),sex char(6) ,score number, sale float,address long);
insert into tab_in values (1,'jay1',21,'male',80,22.12,'南京市雨花区大数据基地1');
insert into tab_in values (2,'may2',22,'fmale',81,30.2,'南京市雨花区大数据基地2');
insert into tab_in values (3,'may3',23,'fmale',83,30.0,'南京市雨花区大数据基地3');
insert into tab_in values (4,'may4',24,'fmale',84,30,'南京市雨花区大数据基地4');
insert into tab_in values (5,'may5',25.00,'fmale',85,30.23,'南京市雨花区大数据基地5');
insert into tab_in values (6,'may6',26,'fmale',86,30.46,'南京市雨花区大数据基地6');
insert into tab_in values (7,'may7',27.00,'fmale',87,39.00,'南京市雨花区大数据基地7');
insert into tab_in values (7,'44',27,'122',87,39.00,'南京市雨花区大数据基地8');

--单表单个字段的in/not in，条件为一个且非空值/隐式类型转换
select * from tab_in a where a.id in (1) order by id desc;
select * from tab_in a where a.id in ('2') order by id desc;
select * from tab_in a where a.sex in ('male') order by id desc;
select * from tab_in a where a.sex in (122) order by id desc;
select * from tab_in a where a.name in ('may5') order by id desc;
select * from tab_in a where a.name in (44) order by id desc;
select * from tab_in a where a.age in (25.00) order by id desc;
select * from tab_in a where a.age in(22) order by id desc;
select * from tab_in a where a.age in('26') order by id desc;
select * from tab_in a where a.age in('27.00') order by id desc;
select * from tab_in a where a.score in(81) order by id desc;
select * from tab_in a where a.score in ('86') order by id desc;
select * from tab_in a where a.sale in(22.12) order by id desc;
select * from tab_in a where a.sale in('30') order by id desc;
select * from tab_in a where a.sale in('39.00') order by id desc;

select * from tab_in a where a.id not in (1) order by id desc;
select * from tab_in a where a.id not in ('2') order by id desc;
select * from tab_in a where a.sex not in ('male') order by id desc;
select * from tab_in a where a.sex not in (122) order by id desc;
select * from tab_in a where a.name not in ('may5') order by id desc;
select * from tab_in a where a.name not in (44) order by id desc;
select * from tab_in a where a.age not in (25.00) order by id desc;
select * from tab_in a where a.age not in(22) order by id desc;
select * from tab_in a where a.age not in('26') order by id desc;
select * from tab_in a where a.age not in('27.00') order by id desc;
select * from tab_in a where a.score not in(81) order by id desc;
select * from tab_in a where a.score not in ('86') order by id desc;
select * from tab_in a where a.sale not in(22.12) order by id desc;
select * from tab_in a where a.sale not in('30') order by id desc;
select * from tab_in a where a.sale not in('39.00') order by id desc;

--单表单个字段的in/not in，条件为多个非空值/隐式类型转换
select * from tab_in a where a.id in (1,2) order by id desc;
select * from tab_in a where a.id in ('1','2') order by id desc;
select * from tab_in a where a.sex in ('male','fmale') order by id desc;
select * from tab_in a where a.sex in (122,123) order by id desc;
select * from tab_in a where a.name in ('may5','jay5') order by id desc;
select * from tab_in a where a.name in (44,46) order by id desc;
select * from tab_in a where a.age in (25.00,23) order by id desc;
select * from tab_in a where a.age in(22,25.00) order by id desc;
select * from tab_in a where a.age in('26','25') order by id desc;
select * from tab_in a where a.age in('27.00','23') order by id desc;
select * from tab_in a where a.score in(81,83) order by id desc;
select * from tab_in a where a.score in ('86','85') order by id desc;
select * from tab_in a where a.sale in(22.12,39.00) order by id desc;
select * from tab_in a where a.sale in('30','22.12') order by id desc;
select * from tab_in a where a.sale in('39.00','30.23') order by id desc;

select * from tab_in a where a.id not in (1,2) order by id desc;
select * from tab_in a where a.id not in ('1','2') order by id desc;
select * from tab_in a where a.sex not in ('male','fmale') order by id desc;
select * from tab_in a where a.sex not in (122,123) order by id desc;
select * from tab_in a where a.name not in ('may5','jay5') order by id desc;
select * from tab_in a where a.name not in (44,46) order by id desc;
select * from tab_in a where a.age not in (25.00,23) order by id desc;
select * from tab_in a where a.age not in(22,25.00) order by id desc;
select * from tab_in a where a.age not in('26','25') order by id desc;
select * from tab_in a where a.age not in('27.00','23') order by id desc;
select * from tab_in a where a.score not in(81,83) order by id desc;
select * from tab_in a where a.score not in ('86','85') order by id desc;
select * from tab_in a where a.sale not in(22.12,39.00) order by id desc;
select * from tab_in a where a.sale not in('30','22.12') order by id desc;
select * from tab_in a where a.sale not in('39.00','30.23') order by id desc;

--单表单个字段的in/not in，条件为多个，同类型/隐式类型混合
select * from tab_in a where a.id in ('1','2',4) order by id desc;
select * from tab_in a where a.sex in ('male','fmale',2) order by id desc;
select * from tab_in a where a.name in ('may5','jay5',44) order by id desc;
select * from tab_in a where a.age in('27.00','23',22,25.00) order by id desc;
select * from tab_in a where a.score in ('86','85',81) order by id desc;
select * from tab_in a where a.sale in('39.00','30.23',22.12) order by id desc;

select * from tab_in a where a.id not in ('1','2',4) order by id desc;
select * from tab_in a where a.sex not in ('male','fmale',2) order by id desc;
select * from tab_in a where a.name not in ('may5','jay5',44) order by id desc;
select * from tab_in a where a.age not in('27.00','23',22,25.00) order by id desc;
select * from tab_in a where a.score not in ('86','85',81) order by id desc;
select * from tab_in a where a.sale not in('39.00','30.23',22.12) order by id desc;

--单表查询，where条件中有多个in / not in /其他条件混合场景
select * from tab_in a where a.id in(1,2,3,'4','5') and a.name not in('jay5') order by id desc;
select * from tab_in a where a.id in(1,2,'3',4,5) and a.name not in('jay5') and a.age > 20 and score=83 order by id desc;

--单表in条件与union /union all 混合查询
select * from tab_in a where a.id in(1,2,'3') 
union
select * from tab_in a where a.id not in(1,2,'3') order by id desc ;
select * from tab_in a where a.id in(1,2,'3') 
union all
select * from tab_in a where a.name in('may5',44) order by id desc;

--多表关联条件涉及到in/not in的隐式类型转换
create table tab_in_cp AS select * from tab_in;
select * from tab_in_cp order by id desc;
select * from tab_in a where a.id in(select id from tab_in_cp) order by id, name desc ;
select * from tab_in a where a.id in(select id from tab_in_cp where age not in('27.00','23',22,25.00)) order by id desc;
select * from tab_in a where a.id not in(select id from tab_in_cp where age in('27.00')) order by id desc;
select * from tab_in a where a.name in (select name from tab_in_cp where age not in('27.00','23',22,25.00)) order by id desc;

--delete/update 包含in/not in
select * from tab_in_cp order by id desc;
update tab_in_cp b set b.address = '南京市雨花区大数据产业基地亚信科技' where b.id in(1,'2',3);
update tab_in_cp b set b.address = '南京市雨花区大数据产业基地亚信科技' where b.sale in('30.46');
update tab_in_cp b set b.address = '南京市雨花区大数据产业基地亚信科技' where b.name in(44);
select * from tab_in_cp order by id desc;

delete from tab_in_cp b where b.id in ('5');
delete from tab_in_cp b where b.name in(44);
delete from  tab_in_cp b where b.sale in ('30.46',30);
delete from tab_in_cp b where b.id in(select a.id from tab_in a  where a.id not in (4,5,'6'));
select * from tab_in_cp order by id desc;

--init table/data
drop table tab_in_cp;
drop table tab_in;

