package clean6265.com.naver.blog.login;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Handler;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;

import java.io.IOException;


//Fun Activity
public class RegisterKidsActivity extends AppCompatActivity implements View.OnClickListener{
    private FirebaseAuth firebaseAuth;

    private TextView textViewUserEmail;
    private Button buttonLogOut;


    private DatabaseReference databaseReference;

    private EditText editTextName, editTextClass, editTextAge,editTextNum, editTextPhoneNum;
    private Button buttonSave,buttonDefault;



    private static final int PICK_IMAGE_REQUEST =234;
    private static final int REQUEST_ENABLE_BT =1;
    private ImageView imageView;
    private Button buttonChoose;
    private int Priority;

    private Uri filepath;

    private StorageReference storageReference;

    BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_kids);

        Priority = getIntent().getIntExtra("Priority", 1);

        firebaseAuth = FirebaseAuth.getInstance();
        databaseReference = FirebaseDatabase.getInstance().getReference();

/*
        if(firebaseAuth.getCurrentUser() == null){                  //로그인된 사람이 없다면
            finish();
            startActivity(new Intent(this, LoginActivity.class));
        }


/*********************** **************************************



        if(databaseReference.getDatabase()!=null){                  //데이터베이스에 정보가 있으면

            startActivity(new Intent(this, KidsGroundActivity.class));                                    //블루투스 연결상태를 확인하여
                                                                        //연결되어있다면, table로 넘어가고
        }
*/


        databaseReference = FirebaseDatabase.getInstance().getReference();
        storageReference= FirebaseStorage.getInstance().getReference();

        editTextClass = (EditText) findViewById(R.id.editTextClass);
        editTextName = (EditText) findViewById(R.id.editTextName);
        editTextAge = (EditText) findViewById(R.id.editTextAge);
        editTextPhoneNum = (EditText) findViewById(R.id.editTextPhoneNum);
        editTextNum=(EditText) findViewById(R.id.editTextNum);
        imageView=(ImageView)findViewById(R.id.imageView);


        buttonSave = (Button) findViewById(R.id.buttonSave);
        buttonDefault = (Button)findViewById(R.id.buttonDefault);
        buttonChoose = (Button)findViewById(R.id.buttonChoose);




        textViewUserEmail =(TextView) findViewById(R.id.textViewUserEmail);


        buttonLogOut = (Button) findViewById(R.id.buttonLogOut);

        buttonLogOut.setOnClickListener(this);
        buttonSave.setOnClickListener(this);
        buttonDefault.setOnClickListener(this);
        buttonChoose.setOnClickListener(this);

    }



    private void showFileChooser(){
        Intent intent =new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent,"Select Image"),PICK_IMAGE_REQUEST);


    }
    private void uploadFile(){
        if(filepath!=null){

            final ProgressDialog progressDialog=new ProgressDialog(this);
            progressDialog.setTitle("사진을 저장중입니다..");
            progressDialog.show();

            FirebaseUser user=firebaseAuth.getCurrentUser();

            StorageReference riversRef =storageReference.child(user.getUid())
                    .child(editTextAge.getText().toString().trim()+"학년")
                    .child(editTextClass.getText().toString().trim()+"반")
                    .child(editTextNum.getText().toString().trim()+"번").child("profile.jpg");

            riversRef.putFile(filepath)
                    .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                            progressDialog.dismiss();
                            Toast.makeText(getApplicationContext(),"사진 업로드 완료",Toast.LENGTH_SHORT).show();
                            saveUserInformation();
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

        }else {
            Toast.makeText(getApplicationContext(),"사진을 등록해주세요.",Toast.LENGTH_LONG).show();

        }

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode==PICK_IMAGE_REQUEST && resultCode==RESULT_OK&&data!=null&&data.getData()!=null){
            filepath=data.getData();

            try {
                Bitmap bitmap= MediaStore.Images.Media.getBitmap(getContentResolver(),filepath);
                imageView.setImageBitmap(bitmap);

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if(requestCode==REQUEST_ENABLE_BT&& resultCode==RESULT_OK){
            // 블루투스연결을 허락하면 블루투스 활성화 상태로 바뀜

            Toast.makeText(this,"블루투스를 성공적으로 켰습니다.",Toast.LENGTH_LONG).show();
            new Handler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    Intent intent1 = new Intent(RegisterKidsActivity.this, StPlutoconListActivity.class);
                    intent1.putExtra("Priority", Priority);
                    startActivity(intent1);
                }
            }, 2000);



        }
        if(requestCode==REQUEST_ENABLE_BT && resultCode == RESULT_CANCELED){
            //블루투스 활성화를 거절한상태
            //초기화면(설정해야함) 으로 넘어간다.
            // 거절했다는 메세지를 띄운다.
            Toast.makeText(this,"블루투스를 거절헀습니다.",Toast.LENGTH_LONG).show();

        }

    }

    private void saveUserInformation(){

        String name = editTextName.getText().toString().trim();
        String classNum =  editTextClass.getText().toString().trim();
        String age = editTextAge.getText().toString().trim();
        String phone = editTextPhoneNum.getText().toString().trim();
        String Number = editTextNum.getText().toString().trim();
        int Priority = 1;                       // 우선순위 default =1

        if(phone.length() ==10 || phone.length() ==11) {

           if(!TextUtils.isEmpty(name)){
               if(!TextUtils.isEmpty(age)){
                   if(!TextUtils.isEmpty(classNum)){
                      if(!TextUtils.isEmpty(Number)){

                          UserKidInformation userKidInformation = new UserKidInformation(name, age, phone, classNum,Number, Priority);


                          FirebaseUser user = firebaseAuth.getCurrentUser();



                          databaseReference.child(user.getUid()).child(editTextAge.getText().toString().trim()+"학년")
                                  .child(editTextClass.getText().toString().trim()+"반")
                                  .child(editTextNum.getText().toString().trim()+"번")
                                  .setValue(userKidInformation);

                          Toast.makeText(this, "사용자 정보를 저장하였습니다.", Toast.LENGTH_SHORT).show();
                          checkBlueTooth();

                      }else Toast.makeText(this,"번호를 입력해주세요.",Toast.LENGTH_LONG).show();
                   }else Toast.makeText(this,"반을 입력해주세요.", Toast.LENGTH_LONG).show();
               }else Toast.makeText(this,"학년 입력해주세요.", Toast.LENGTH_LONG).show();
           }else Toast.makeText(this,"이름을 입력해주세요.", Toast.LENGTH_LONG).show();


        }else {
            Toast.makeText(this,"핸드폰 번호를 확인해 주세요", Toast.LENGTH_LONG).show();
        }

    }

    private void checkBlueTooth(){
        mBluetoothAdapter= BluetoothAdapter.getDefaultAdapter();
        if(mBluetoothAdapter==null){
            //해당 기기가 블루투수를 지원안함
            finish();
        }

        else{
            if(!mBluetoothAdapter.isEnabled()){
                //블루투스를 지원하지만 비활성화 상태인 경우
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent,REQUEST_ENABLE_BT);               //요청
            }else{
            
                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        Intent intent1 = new Intent(RegisterKidsActivity.this, StPlutoconListActivity.class);
                        startActivity(intent1);
                    }
                }, 2000);
            }


        }

    }
    public void onBackPressed(){
        startActivity(new Intent(this, LoginActivity.class));
        super.onBackPressed();
    }

    @Override
    public void onClick(View view) {


        if(view == buttonLogOut){
            firebaseAuth.signOut();
            finish();
            startActivity(new Intent(this, LoginActivity.class));

        }
        if (view == buttonSave){
            uploadFile();
            // uploadFile -> saveuserinformation -> checkbluetooth -> 액티비티전환



        }

        if(view==buttonDefault){
            finish();
            startActivity(new Intent(getApplicationContext(),LoginActivity.class));

        }
        if(view==buttonChoose){
            showFileChooser();

        }

    }



}
