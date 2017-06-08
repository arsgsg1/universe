package clean6265.com.naver.blog.login;

import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.EditText;
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

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class KidsGroundActivity extends AppCompatActivity implements View.OnClickListener{


    private DatabaseReference databaseReference;
    private StorageReference storageReference;

    private AlertDialog mDialog = null;
    private TextView  mTextQuest;
    private EditText mTextAnswer;
    private VideoView videoViewData;
    private Button buttonSubmit;
    private WebView imageViewData;
    private int Priority;



    String RealAnswer, NextHint, beaconUUID;    //SearchBeaconActivity에서 가져온 UUID를 저장하는 beaconUUID변수
    final int CORRECT_ANSWER =1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_kidsground);

        beaconUUID = getIntent().getStringExtra("beaconUUID");
        Priority = getIntent().getIntExtra("Priority", 1);

        databaseReference = FirebaseDatabase.getInstance().getReference("BeaconID");
        storageReference = FirebaseStorage.getInstance().getReference("BeaconID");
        mTextQuest=(TextView)findViewById(R.id.mTextQuest);
        mTextAnswer = (EditText)findViewById(R.id.mTextAnswer);
        videoViewData = (VideoView)findViewById(R.id.videoViewData);
        buttonSubmit = (Button)findViewById(R.id.buttonSubmit);
        imageViewData = (WebView) findViewById(R.id.imageViewData);

        buttonSubmit.setOnClickListener(this);


        DatabaseReference mRef = databaseReference.child(beaconUUID);
        mRef.child("Ans").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                RealAnswer = dataSnapshot.getValue(String.class);
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        });
        mRef.child("Hint").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                NextHint = dataSnapshot.getValue(String.class);
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        });
        mRef.child("Pri").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Priority = dataSnapshot.getValue(Integer.class);
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });

    }

    @Override
    protected void onStart() {
        super.onStart();


        //String FilePath = "비콘ID"+"/KakaoTalk_Video_20161227_1124_23_051.mp4";
        StorageReference strRef = storageReference.child(beaconUUID).child("image");
        final StorageReference strRef2 = storageReference.child(beaconUUID).child("video");




        //Toast.makeText(this, strRef.getPath().toString(),Toast.LENGTH_LONG).show();
        strRef.getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
            @Override
            public void onSuccess(Uri uri) {
               imageViewData.setVisibility(View.VISIBLE);
                videoViewData.setVisibility(View.GONE);

                imageViewData.setFocusable(false);
                //imageViewData.getSettings().setLayoutAlgorithm(WebSettings.LayoutAlgorithm.SINGLE_COLUMN);
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

                        MediaController mediaController = new MediaController(KidsGroundActivity.this);
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
        DatabaseReference mRef = databaseReference.child(beaconUUID);

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






    @Override
    public void onClick(View view) {

        if(view==buttonSubmit){

            String MyAnswer = mTextAnswer.getText().toString().trim();


            if(MyAnswer.equals(RealAnswer)){
                mDialog = createDialog(CORRECT_ANSWER);
                mDialog.show();
            }else{
                mDialog = createDialog(0);
                mDialog.show();
            }

        }
    }

    private AlertDialog createDialog(final int correctCode) {
        AlertDialog.Builder ab = new AlertDialog.Builder(this);

        DatabaseReference mRef = databaseReference.child(beaconUUID);

        mRef.child("Hint").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                NextHint = dataSnapshot.getValue(String.class);
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        });





        if(CORRECT_ANSWER == correctCode) {
            ab.setTitle("축하합니다! 정답입니다!");
            ab.setMessage(NextHint);
        }else{
            ab.setTitle("다시 생각해보세요");

        }

        ab.setCancelable(false);

        ab.setPositiveButton("확인", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                if( CORRECT_ANSWER == correctCode) {
                    Toast.makeText(getApplicationContext(), "다음 비콘을 찾는다.", Toast.LENGTH_LONG).show();
                    Intent intent = new Intent(KidsGroundActivity.this, StPlutoconListActivity.class);
                    intent.putExtra("Priority", Priority+1);  //인텐트로 우선순위를 전달한다.
                    startActivity(intent);
                    finish();
                }
                setDismiss(mDialog);
            }
        });

        ab.setNegativeButton("취소", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                setDismiss(mDialog);
            }
        });

        return ab.create();
    }


    private void setDismiss(Dialog dialog){
        if(dialog != null && dialog.isShowing())
            dialog.dismiss();
    }


}