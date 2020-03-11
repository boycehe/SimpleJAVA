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
	
	public void updateNumber(int num){
		this.number = num;
	}
 
    public int ageOfMajority(){
        int m1 = 19;
        return m1;
    }

	public void main() {
		
		Student stud = new Student();
		stud.updateNumber(12);
		stud.age = stud.ageOfMajority();
		stud.sex = 1;
		stud.birthday = "2020-01-02";
		stud.study("english");
		printf(stud.age);
        printf(stud.sex);
		printf(stud.currentStudyLesson);
	}	
	
}
