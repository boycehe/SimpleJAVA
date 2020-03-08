class Person {
	
	int age,sex;
	String name;
	String birthday;
	
}
class Student extends Person {

	int number;
	int grade;
	String currentStudyLesson;
	
	public void study(String lesson){
		this.currentStudyLesson = lesson;	
	}
	
	public int getNumber(){
		return this.number;
	}
	
	public void updateNumber(int num){
		this.number = num;
	}
	
	public void main() {
		
		Student stud = new Student();
		stud.updateNumber(12);
		stud.age = 12;
		stud.sex = 1;
		stud.birthday = "2020-01-02";
		stud.study("english");
		printf(stud.age);
		printf(stud.currentStudyLesson);
	}	
	
}