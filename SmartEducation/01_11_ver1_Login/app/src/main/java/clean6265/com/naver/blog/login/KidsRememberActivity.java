package clean6265.com.naver.blog.login;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.MediaController;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.VideoView;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;

public class KidsRememberActivity extends AppCompatActivity implements View.OnClickListener {



    private DatabaseReference databaseReference;
    private StorageReference storageReference;

    private TextView mTextQuest;
    private VideoView videoViewData;
    private Button buttonSubmit;
    private WebView imageViewData;
    private TextView Hint;
    private TextView Answer;
    private int Priority;

    String RealAnswer, NextHint, beaconUUID;    //SearchBeaconActivity에서 가져온 UUID를 저장하는 beaconUUID변수


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_kids_remember);

        beaconUUID = getIntent().getStringExtra("beaconUUID");
        Priority = getIntent().getIntExtra("Priority", 1);

        Hint = (TextView)findViewById(R.id.mTextAnswer1);
        Answer = (TextView)findViewById(R.id.mTextAnswer2);

        databaseReference = FirebaseDatabase.getInstance().getReference("BeaconID");
        storageReference = FirebaseStorage.getInstance().getReference("BeaconID");
        mTextQuest=(TextView)findViewById(R.id.mTextQuest);
        videoViewData = (VideoView)findViewById(R.id.videoViewData);
        buttonSubmit = (Button)findViewById(R.id.buttonSubmit);
        imageViewData = (WebView) findViewById(R.id.imageViewData);

        buttonSubmit.setOnClickListener(this);


        DatabaseReference mRef = databaseReference.child(beaconUUID);
        mRef.child("Ans").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Answer.setText(dataSnapshot.getValue(String.class));
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        });
        mRef.child("Hint").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Hint.setText(dataSnapshot.getValue(String.class));
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        });


        //String FilePath = "비콘ID"+"/KakaoTalk_Video_20161227_1124_23_051.mp4";
        //이부분에 동영상과 사진파일 주소를 넣어야한다.
        StorageReference strRef = storageReference.child(beaconUUID).child("image");
        final StorageReference strRef2 = storageReference.child(beaconUUID).child("video");

        //StorageReference strRef = storageReference.child(beaconUUID).child("image");
        //final StorageReference strRef2 = storageReference.child(beaconUUID).child("video");


        strRef.getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
            @Override
            public void onSuccess(Uri uri) {
                imageViewData.setVisibility(View.VISIBLE);
                videoViewData.setVisibility(View.GONE);

                imageViewData.setFocusable(false);

                if(imageViewData!=null) {
                    imageViewData.loadUrl(uri.toString());
                    WebSettings ws = imageViewData.getSettings();
                    ws.setDefaultFontSize(8);
                    //imageViewData.getSettings().setDefaultZoom(WebSettings.ZoomDensity.FAR); // 화면을 유지
                    // set the scale
                    imageViewData.setInitialScale(10); // 35%
                    imageViewData.getSettings().setSupportZoom(true);
                    imageViewData.getSettings().setBuiltInZoomControls(true);
                    imageViewData.getSettings().setUseWideViewPort(true);
                }
            }
        }).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {

                strRef2.getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
                    @Override
                    public void onSuccess(Uri uri) {
                        videoViewData.setVideoURI(uri);
                        videoViewData.setVisibility(View.VISIBLE);
                        imageViewData.setVisibility(View.INVISIBLE);
                        MediaController mediaController = new MediaController(KidsRememberActivity.this);
                        mediaController.setAnchorView(videoViewData);
                        mediaController.setPadding(0, 0, 0, 80); //상위 레이어의 바닥에서 얼마 만큼? 패딩을 줌
                        videoViewData.setMediaController(mediaController);
                        videoViewData.start();
                    }
                }).addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        Toast.makeText(getApplicationContext(), "두번이나 다시해보자",Toast.LENGTH_LONG).show();
                    }
                });
            }
        });

        //////////////// 사진 or 동영상 외의 정보들을 가져온다
        String s [] = new String [2];
        s[0] = "Quest";
        s[1] = "Ans";

        for(int i =0; i<s.length;i++) {
            //database의 내 ID 속 Default 폴더로 접근
            switch (i) {
                case 0 : mRef.child(s[0]).addValueEventListener(new ValueEventListener() {
                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {
                        String temp = dataSnapshot.getValue(String.class);
                        mTextQuest.setText(temp);
                    }

                    @Override
                    public void onCancelled(DatabaseError databaseError) {
                    }
                });
                    break;
                case 1 : mRef.child(s[1]).addValueEventListener(new ValueEventListener() {
                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {
                        String temp = dataSnapshot.getValue(String.class);
                    }
                    @Override
                    public void onCancelled(DatabaseError databaseError) {
                    }
                });
                    break;
            }
        }

    }
    public void onBackPressed() {
        Intent intent = new Intent(this, OldQuestActivity.class);
        intent.putExtra("Priority", Priority);
        startActivity(intent);
        finish();
        super.onBackPressed();
    }


    @Override
    public void onClick(View view) {
        if(view==buttonSubmit){
            Intent intent = new Intent(this, OldQuestActivity.class);
            intent.putExtra("Priority", Priority);
            startActivity(intent);
            finish();
        }
    }
}