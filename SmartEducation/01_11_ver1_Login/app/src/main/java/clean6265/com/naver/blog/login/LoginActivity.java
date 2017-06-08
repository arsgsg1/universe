package clean6265.com.naver.blog.login;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.graphics.drawable.ColorDrawable;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

// 선생님용 로그인 Activity
public class LoginActivity extends AppCompatActivity implements View.OnClickListener{


    private Button buttonSignIn;
    private Button buttonGotoKids;
    private EditText editTextEmail;
    private EditText editTextPassword;
    private TextView textViewSignup;
    private static final int REQUEST_ENABLE_BT =1;

    private FirebaseAuth firebaseAuth;
    private DatabaseReference databaseReference;
    private ProgressDialog progressDialog;
    BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_login);


        firebaseAuth = FirebaseAuth.getInstance();
        databaseReference= FirebaseDatabase.getInstance().getReference();

        /*
        if(firebaseAuth.getCurrentUser() != null){                              //현재 로그인된 유저가 있으면

            finish();
            startActivity(new Intent(getApplicationContext(),SaveTeacherDActivity.class));       //default 창으로


        }*/

        editTextEmail = (EditText) findViewById(R.id.editTextEmail);
        editTextPassword = (EditText) findViewById(R.id.editTextPassword);
        buttonSignIn = (Button) findViewById(R.id.buttonSignin);
        buttonGotoKids= (Button)findViewById(R.id.buttonGotoKids);
        textViewSignup = (TextView) findViewById(R.id.textViewSignup);

        progressDialog = new ProgressDialog(this);

        buttonSignIn.setOnClickListener(this);
        buttonGotoKids.setOnClickListener(this);
        textViewSignup.setOnClickListener(this);

    }

    private void userLogin(){
        String email = editTextEmail.getText().toString().trim();
        String password = editTextPassword.getText().toString().trim();

        if(TextUtils.isEmpty(email)){
            Toast.makeText(this,"Please enter your email",Toast.LENGTH_LONG).show();

            return;
        }
        if(TextUtils.isEmpty(password)){
            Toast.makeText(this,"Please enter your password",Toast.LENGTH_LONG).show();

            return;
        }

        progressDialog.setMessage("Login...");
        progressDialog.show();

        firebaseAuth.signInWithEmailAndPassword(email, password)                        //firevbaseAuth 내 저장된 이메일과 패스워드 비교
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        progressDialog.dismiss();


                        if(task.isSuccessful()){                                    //제대로 입력한경우

                            if(databaseReference.getDatabase()==null) {         //database 내부에 정보가 없으면
                                finish();
                                startActivity(new Intent(getApplicationContext(), RegisterTeacherActivity.class));

                            }else checkBlueTooth();

                        }else {
                            Toast.makeText(LoginActivity.this, "Check Your ID or Password, Please", Toast.LENGTH_LONG).show();
                        }



                    }
                });

    }
//굳이 로그인을 할 필요가 있는가?
    private void studentLogin(){
//학생들의 정보가 담겨져 있는 로그인, 데이터베이스에서 일괄적으로 한번에 관리하기 위해 임의로 아이디와 패스워드를 지정하여 설정해주었습니다.
        //현재 Storage에는 email과 대응되는 디렉토리 안에 학생들이 올린 파일이 존재하고 있습니다.
        //즉, email과 password로 임의로 로그인하여 학생들이 올린 파일을 열람이 가능하게 하는 것입니다.
        String email = "student@student.com";
        String password = "qwer123";

         firebaseAuth.signInWithEmailAndPassword(email, password)                        //firevbaseAuth 내 저장된 이메일과 패스워드 비교
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {

                    }
                });



    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(requestCode==REQUEST_ENABLE_BT&& resultCode==RESULT_OK){
            // 블루투스연결을 허락하면 블루투스 활성화 상태로 바뀜
            Toast.makeText(this,"블루투스를 사용합니다.",Toast.LENGTH_LONG).show();
            startActivity(new Intent(this,DataUploadActivity.class));

        }
        if(requestCode==REQUEST_ENABLE_BT && resultCode == RESULT_CANCELED){
            //블루투스 활성화를 거절한상태
            //초기화면(설정해야함) 으로 넘어간다. // 지금은 잠시 Default 설정창으로
            // 거절했다는 메세지를 띄운다.
            Toast.makeText(this,"블루투스를 거절헀습니다..",Toast.LENGTH_LONG).show();
            //startActivity(new Intent(getApplicationContext(),SaveTeacherDActivity.class));

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

                //블루투스를 지원하고 활성상태인 경우

                startActivity(new Intent(this, DataUploadActivity.class));

                // Table activitiy로 넘어간다.
            }


        }

    }

    @Override
    public void onClick(View view) {
        if(view == buttonSignIn){
            userLogin();

        }
        if(view == textViewSignup){
            startActivity(new Intent(this,RegisterTeacherActivity.class));
            finish();
        }

        if(view == buttonGotoKids){
            studentLogin();
           startActivity(new Intent(this,RegisterKidsActivity.class));
            finish();
        }
    }
}
