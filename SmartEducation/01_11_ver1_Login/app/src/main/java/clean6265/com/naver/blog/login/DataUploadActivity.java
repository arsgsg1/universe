package clean6265.com.naver.blog.login;

import android.app.Dialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;
import com.kongtech.plutocon.sdk.Plutocon;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class DataUploadActivity extends AppCompatActivity implements View.OnClickListener{


    private FirebaseAuth firebaseAuth;
    private String beaconUUID =null;
    private String Question =null;
    private int Priority = 0;

    private String FileType=null;
    private ArrayList arrayList = new ArrayList();
    private Map<String, AnswerHintInformation> userKidInformationMap;


    private TextView beacon;
    private Button buttonSearch, buttonSaveData,buttonList;
    private DatabaseReference databaseReference;
    private RelativeLayout activity_data_upload;
    private EditText editTextAns, editTextHint, editTextQuest,editTextPri;
    private TextView editTextData;
    private AlertDialog mDialog = null;
    private Uri filepath;
    private StorageReference storageReference;
    private DatabaseReference mRef;

    ///////////상수 영역/////////
    private static final int PICK_IMAGE_REQUEST =234;
    private static final int REQUEST_ENABLE_BT =1;
    private static final int SELECT_BEACON = 31;



    BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

    //ListData oldData = null;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_data_upload);


        firebaseAuth = FirebaseAuth.getInstance();
        databaseReference = FirebaseDatabase.getInstance().getReference();
        userKidInformationMap = new HashMap<String, AnswerHintInformation>();

        if(firebaseAuth.getCurrentUser() == null){                              //현재 로그인된 유저가 없다면
            finish();
            startActivity(new Intent(this, LoginActivity.class));               //LoginActivity로 이동
        }

        storageReference= FirebaseStorage.getInstance().getReference();


        editTextAns = (EditText)findViewById(R.id.editTextAns);
        editTextData = (TextView) findViewById(R.id.editTextData);
        editTextHint = (EditText)findViewById(R.id.editTextHint);
        editTextQuest = (EditText)findViewById(R.id.editTextQuest);
        editTextPri =  (EditText)findViewById(R.id.editTextPri);
        // RadioImgVideo = (RadioGroup)findViewById(R.id.RadioImgVideo);
        buttonSearch =(Button)findViewById(R.id.buttonSearch);
        buttonSaveData =(Button)findViewById(R.id.buttonSaveData);
        buttonList = (Button)findViewById(R.id.buttonList);
        // RadioLayout = (LinearLayout)findViewById(R.id.RadioLayout);
        // buttonSelect =(Button)findViewById(R.id.buttonSelect);
        activity_data_upload =(RelativeLayout)findViewById(R.id.activity_data_upload);

        beacon = (TextView)findViewById(R.id.textView7);



        buttonSaveData.setOnClickListener(this);
        buttonSearch.setOnClickListener(this);
        //buttonSelect.setOnClickListener(this);
        buttonList.setOnClickListener(this);
        editTextData.setOnClickListener(this);

        mRef = FirebaseDatabase.getInstance().getReference("BeaconID");
        mRef.addValueEventListener(new ValueEventListener() {   //DataBase의 정보를 모두 긁어옵니다. 또한 DataBase의 정보가 바뀔때마다 동작합니다.
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                if(dataSnapshot != null && dataSnapshot.getValue() != null) {
                    for (DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                        //getChildren은 DataBase의 지정된 항목의 iterator를 반환합니다.
                        userKidInformationMap.put(postSnapshot.getKey(), postSnapshot.getValue(AnswerHintInformation.class));
                        //BeaconUUID와 UserKidInformation형태의 value를 반홥합니다.
                    }
                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.e("데이터베이스에러", "Failed");
            }
        });
    }




    private void showFileChooser(){                              //사진파일 선택

        Intent intent =new Intent();
        intent.setType(FileType);
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent,"Select Data"),PICK_IMAGE_REQUEST);
    }


    private void uploadFile(){
        if(filepath!=null){                                         //저장될 자료가 선택된경우

            final ProgressDialog progressDialog=new ProgressDialog(this);
            progressDialog.setTitle("자료를 업로드 중입니다..");
            progressDialog.show();

            FirebaseUser user=firebaseAuth.getCurrentUser();
            StorageReference riversRef =storageReference.child("BeaconID");


            StorageReference ImageDeleteRef = storageReference.child("BeaconID");
            final StorageReference VideoDeleteRef = storageReference.child("BeaconID");



            ImageDeleteRef.child(beaconUUID).child("image").delete().addOnSuccessListener(new OnSuccessListener<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    //Toast.makeText(getApplicationContext(), "이미지제거성공", Toast.LENGTH_SHORT).show();
                }
            }).addOnFailureListener(new OnFailureListener() {
                @Override
                public void onFailure(@NonNull Exception e) {
                    //Toast.makeText(getApplicationContext(), "이미지제거실패", Toast.LENGTH_SHORT).show();

                    VideoDeleteRef.child(beaconUUID).child("video").delete().addOnSuccessListener(new OnSuccessListener<Void>() {
                        @Override
                        public void onSuccess(Void aVoid) {
                            //Toast.makeText(getApplicationContext(), "동영상제거성공", Toast.LENGTH_SHORT).show();
                        }
                    }).addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception e) {
                            // Toast.makeText(getApplicationContext(), "동영상제거실패", Toast.LENGTH_SHORT).show();
                        }
                    });


                }
            });






            if(FileType.equals("image/*")){
                riversRef = riversRef.child(beaconUUID+"/image");
                Toast.makeText(getApplicationContext(),riversRef.getPath().toString(),Toast.LENGTH_LONG).show();
            }else if (FileType.equals("video/*")){
                riversRef = riversRef.child(beaconUUID+"/video");
                Toast.makeText(getApplicationContext(),riversRef.getPath().toString(),Toast.LENGTH_LONG).show();
            }

            riversRef.putFile(filepath)
                    .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                            progressDialog.dismiss();
                            Toast.makeText(getApplicationContext(), "업로드 하였습니다.", Toast.LENGTH_LONG).show();


                        }
                    })
                    .addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception exception) {
                            progressDialog.dismiss();
                            Toast.makeText(getApplicationContext(),exception.getMessage(),Toast.LENGTH_LONG).show();
                        }
                    })
                    .addOnProgressListener(new OnProgressListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onProgress(UploadTask.TaskSnapshot taskSnapshot) {
                            double progress =(100.0*taskSnapshot.getBytesTransferred())/taskSnapshot.getTotalByteCount();
                            progressDialog.setMessage(((int) progress)+"%Uploaded...");
                        }
                    });
            ;

            SaveAnswerHint();
        }else {


        }

    }

    private void SaveAnswerHint(){

        String Ans = editTextAns.getText().toString().trim();
        String Hint =  editTextHint.getText().toString().trim();
        String Quest = editTextQuest.getText().toString().trim();
       // String Pri = editTextPri.getText().toString().trim();
        int Pri = Integer.parseInt(editTextPri.getText().toString().trim());
        //String Pri = editTextPri.getText().toString().trim();




        AnswerHintInformation answerHintInformation = new AnswerHintInformation(Ans, Hint,Quest, Pri);



        databaseReference.child("BeaconID").child(beaconUUID).setValue(answerHintInformation);




    }





    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {              //핸드폰에서 사진가져옴
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode==PICK_IMAGE_REQUEST && resultCode==RESULT_OK&&data!=null&&data.getData()!=null){
            filepath=data.getData();

            editTextData.setText(filepath.getPath());
        }


        if(requestCode==REQUEST_ENABLE_BT&& resultCode==RESULT_OK){
            // 블루투스연결을 허락하면 블루투스 활성화 상태로 바뀜

            Toast.makeText(this,"블루투스를 사용합니다.",Toast.LENGTH_LONG).show();
            //startActivity(new Intent(this,KidsGroundActivity.class));

        }
        if(requestCode==REQUEST_ENABLE_BT && resultCode == RESULT_CANCELED){
            //블루투스 활성화를 거절한상태
            //초기화면(설정해야함) 으로 넘어간다.
            // 거절했다는 메세지를 띄운다.
            Toast.makeText(this,"블루투스를 거절했습니다.",Toast.LENGTH_LONG).show();

        }
        if(requestCode == SELECT_BEACON && resultCode == RESULT_OK){
            Plutocon plutocon = (Plutocon) data.getParcelableExtra("PLUTOCON");
            beaconUUID =plutocon.getUuid().toString();
            Toast.makeText(this, beaconUUID, Toast.LENGTH_SHORT).show();
            beacon.setText("비콘: " + beaconUUID);
            //RadioLayout.setVisibility(View.VISIBLE);
            //RadioLayout.bringChildToFront();
            //  mDialog=createDialog();
            // mDialog.show();
        }

    }





    @Override
    public void onClick(View view) {
        Question = editTextQuest.getText().toString().trim();
        //Toast.makeText(this,editTextPri.getText().toString(),Toast.LENGTH_SHORT).show();
//
        String DataPath = editTextData.getText().toString().trim();


        if(view == editTextData){

            mDialog=createDialog();
            mDialog.show();
        }


        else if(view == buttonSearch){
            startActivityForResult(new Intent(DataUploadActivity.this, PlutoconListActivity.class), SELECT_BEACON);
        }
        else if(view == buttonSaveData){        //저장하기 버튼을 누르면
            //현재 구현해야할 부분은 우선순위가 무조건 겹치지 않아야 합니다. 그러기 위해서 데이터베이스에서 우선순위를 전부 불러와 비교해야 합니다.
            //

            Priority = Integer.parseInt(editTextPri.getText().toString());
            Question = editTextQuest.getText().toString();
            //데이터베이스에 있는 UUID(Key)와 UserKidInformation(Value)를 전부 Map의 형태로 가져온 상태이다.

            //데이터베이스에서 BeaconID하위 항목의 정보들을 모두 긁어온 부분
            Log.e("MapSize", String.valueOf(userKidInformationMap.size()));

            if( beaconUUID!= null && !(Question.isEmpty()) && !(DataPath.isEmpty())){
                //BeaconUUID가 탐색이 되었고, 문제의 이름이 작성되었고, 올릴 파일이 지정되었을때

                for(String uuid : userKidInformationMap.keySet()){  //keySet메소드는 map의 key를 차례로 반환합니다.
                    //이 부분에서는 getKey로 key와 대응되는 value(UserKidInformation class) 를 반환하여 우선순위를 비교하게 할 것입니다.
                    if(beaconUUID == uuid) continue;    //선생님이 올린 데이터의 우선순위를 수정하지 못한다면 이상하기 때문에 수정함
                    if(Priority == userKidInformationMap.get(uuid).Pri) {
                        //불러온 비콘들에 저장된 우선순위와 새로 저장할 데이터의 우선순위가 겹칠때 함수를 종료하도록 합니다.
                        Toast.makeText(this, "우선순위가 겹칩니다. 다른 우선순위를 설정해주세요", Toast.LENGTH_SHORT).show();
                        return;
                    }
                }

                ListData listData = null;
                listData = new ListData();
                listData.mTitle = Question;
                listData.mDate = beaconUUID;
                listData.mPri = Priority;






                    //////////////////// storage 초기화 //////////////////////////////////
                    StorageReference ImageDeleteRef = storageReference.child("BeaconID");
                    final StorageReference VideoDeleteRef = storageReference.child("BeaconID");



                    ImageDeleteRef.child(beaconUUID).child("image").delete().addOnSuccessListener(new OnSuccessListener<Void>() {
                        @Override
                        public void onSuccess(Void aVoid) {
                            //Toast.makeText(getApplicationContext(), "이미지제거성공", Toast.LENGTH_SHORT).show();
                        }
                    }).addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception e) {
                            //Toast.makeText(getApplicationContext(), "이미지제거실패", Toast.LENGTH_SHORT).show();

                            VideoDeleteRef.child(beaconUUID).child("video").delete().addOnSuccessListener(new OnSuccessListener<Void>() {
                                @Override
                                public void onSuccess(Void aVoid) {
                                    //Toast.makeText(getApplicationContext(), "동영상제거성공", Toast.LENGTH_SHORT).show();
                                }
                            }).addOnFailureListener(new OnFailureListener() {
                                @Override
                                public void onFailure(@NonNull Exception e) {
                                    // Toast.makeText(getApplicationContext(), "동영상제거실패", Toast.LENGTH_SHORT).show();
                                }
                            });


                        }
                    });

                    ///////////////////////////////// database초기화 /////////////////////////////////////////////////

                   /* DatabaseReference dataDeleRef = databaseReference.child("BeaconID");

                    dataDeleRef.removeValue().addOnSuccessListener(new OnSuccessListener<Void>() {
                        @Override
                        public void onSuccess(Void aVoid) {

                        }
                    }).addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception e) {

                        }
                    });*/


                Iterator<ListData> it = arrayList.iterator();
                while(it.hasNext()){
                    ListData data = it.next();
                    if(data.mDate == beaconUUID){
                        it.remove();
                    }
                }

                arrayList.add(listData);

                uploadFile();
            }else {
                if(Question.isEmpty())
                    Toast.makeText(this, "문제를 입력해주세요", Toast.LENGTH_SHORT).show();
                if(beaconUUID == null)
                    Toast.makeText(this, "비콘을 탐색해주세요", Toast.LENGTH_SHORT).show();
                if(DataPath.isEmpty())
                    Toast.makeText(this, "파일을 선택해주세요", Toast.LENGTH_SHORT).show();


            }

        }
        else if(view == buttonList){
            startActivity(new Intent(getApplicationContext(), DataInBeaconActivity.class));
        }


    }

    private AlertDialog createDialog() {
        AlertDialog.Builder ab = new AlertDialog.Builder(this);
        ab.setTitle("어떤 형태의 파일을 업로드 하시겠습니까?");
        ab.setCancelable(false);

        ab.setPositiveButton("사진파일", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                FileType="image/*";
                showFileChooser();
                setDismiss(mDialog);
            }
        });

        ab.setNegativeButton("동영상파일", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                FileType ="video/*";
                showFileChooser();
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