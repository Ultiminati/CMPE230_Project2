; ModuleID = 'advcalc2ir'
declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
%siu = alloca i32
%siuuu = alloca i32
store i32 11, i32* %siu
store i32 7, i32* %siuuu
%1 = load i32, i32* %siu
%2 = load i32, i32* %siuuu
%3 = sdiv i32 %1,%2
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %3 )
%5 = load i32, i32* %siu
%6 = load i32, i32* %siuuu
%7 = mul i32 %5,%6
store i32 %7, i32* %siu
%8 = load i32, i32* %siu
%9 = load i32, i32* %siu
%10 = sub i32 %8,%9
%11 = load i32, i32* %siu
%12 = load i32, i32* %siu
%13 = mul i32 %11,%12
%14 = load i32, i32* %siu
%15 = sdiv i32 %13,%14
%16 = add i32 %10,%15
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %16 )
ret i32 0
}