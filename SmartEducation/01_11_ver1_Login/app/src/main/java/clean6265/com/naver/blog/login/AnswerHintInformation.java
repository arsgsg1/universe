package clean6265.com.naver.blog.login;

import java.io.Serializable;

/**
 * Created by Jiyoung on 2016-12-20.
 */
//데이터 베이스에 올리기 위한 클래스 입니다.
    //해당 클래스의 format에 따라 DB의 구조도 달라집니다.
    //Java에선 객체를 전달하기 위해선 직렬화 과정이 꼭 필요하기 때문에, 직렬화 인터페이스를 상속하여 구현합니다.
public class AnswerHintInformation implements Serializable{

    public String Ans;  //key == Ans, value
    public String Hint;
    public String Quest;
    private static final long serialVersionUID = 1210L;
    public int Pri;

    public AnswerHintInformation(){




    }

    public AnswerHintInformation(String Ans, String Hint, String Quest, int Pri){
        this.Quest = Quest;
        this.Ans = Ans;
        this.Hint = Hint;
        this.Pri=Pri;


    }

}
