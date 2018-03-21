set grammar to oracle;
select nanvl('nan'::numeric(5,2),100) from dual;
select nanvl('nan'::numeric(5,2),3.178) from dual;
select nanvl('nan'::numeric(5,2),'20') from dual;
select nanvl('nan'::numeric(5,2),'20.5') from dual;
select nanvl('nan'::numeric(5,2),2*3) from dual;
select nanvl('nan'::numeric(5,2),exp(2)) from dual;
select nanvl('nan'::numeric(5,2),'s') from dual;
select nanvl(5,4.7) from dual;
select nanvl(5.8,4.7) from dual;
select nanvl('8',4.7) from dual;
select nanvl('4.8',4.7) from dual;
select nanvl('s',4.7) from dual;
select nanvl(exp(2),4.7) from dual;
select nanvl(2*5.7,4.7) from dual;
select nanvl(exp(2),sin(3)) from dual;
select nanvl('','') from dual;
select nanvl('',null) from dual;
select nanvl(null,4) from dual;
select nanvl(4.5,null) from dual;
