package clean6265.com.naver.blog.login;

import android.graphics.drawable.Drawable;

import java.io.Serializable;
import java.text.Collator;
import java.util.Comparator;
import java.util.List;

/**
 * Created by Jiyoung on 2016-12-30.
 */

public class ListData implements Serializable{
    //안드로이드에서 객체를 전달하기 위해서는 putExtra를 통해 전달해야합니다. 그러나 이 경우
    //객체의 직렬화라는 과정을 거쳐야만 합니다.
    //객체의 직렬화를 구현하기 위해서는 Serializable이란 interface를 구현해야 합니다.

    /**
     * 리스트 정보를 담고 있을 객체 생성
     */
    private static final long serialVersionUID = 1209L;
    // 아이콘
  //  public Drawable mIcon;

    // 문제이름
    public String mTitle;

    // 비콘의 UUID
    public String mDate;

    //비콘의 우선순위
    public int mPri;

    public ListData(){

    }
    public ListData(String mTitle, String mDate, int mPri){
        this.mTitle = mTitle;
        this.mDate = mDate;
        this.mPri = mPri;
    }

    /**
     * 알파벳 이름으로 정렬
     */

    public static final Comparator<ListData> ALPHA_COMPARATOR = new Comparator<ListData>() {
        private final Collator sCollator = Collator.getInstance();

        @Override
        public int compare(ListData mListDate_1, ListData mListDate_2) {
            return sCollator.compare(mListDate_1.mTitle, mListDate_2.mTitle);
        }
    };



}
