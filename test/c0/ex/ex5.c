/* 例子5：PL/0程序调用过程以call开头，C程序无此前缀，与变量赋值开头相同，因而LL(1)分析时要变换 */
/*procedure b;
begin
end;
begin
call b;
end.*/

void b()
{
}

void main()
{
	b();
}

