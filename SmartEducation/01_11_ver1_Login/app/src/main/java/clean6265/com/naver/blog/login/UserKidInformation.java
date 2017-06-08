package clean6265.com.naver.blog.login;

/**
 * Created by Jiyoung on 2016-11-04.
 */
//

public class UserKidInformation {
    public String name;
    public String age;
    public String classNum;
    public String Number;
    public String phone;
    public int priority;


    public UserKidInformation() {

    }

    public UserKidInformation(String name, String age, String phone, String classNum, String Number, int Priority ) {
        this.name = name;
        this.age = age;
        this.classNum = classNum;
        this.Number = Number;
        this.phone = phone;
        this.priority = Priority;

    }
}
