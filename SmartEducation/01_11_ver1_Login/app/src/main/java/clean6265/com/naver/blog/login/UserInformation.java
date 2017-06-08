package clean6265.com.naver.blog.login;

/**
 * Created by Jiyoung on 2016-11-04.
 */
//데이터베이스에 저장될 목적의 클래스입니다.
public class UserInformation {

    public String name;
    public String address;
    public String age;
    public String phone;

    public UserInformation(){




    }

    public UserInformation(String name, String age,String phone,String address ) {
        this.name = name;
        this.address = address;
        this.age = age;
        this.phone = phone;
    }
}
