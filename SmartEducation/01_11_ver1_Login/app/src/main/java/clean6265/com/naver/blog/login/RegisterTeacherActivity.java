package clean6265.com.naver.blog.login;

import android.app.ProgressDialog;
import android.content.Intent;
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
// 선생님용 회원가입 Activity
public class RegisterTeacherActivity extends AppCompatActivity implements View.OnClickListener{


    private Button buttonRegister;
    private EditText editTextEmail;
    private EditText editTextPassword;
    private TextView textViewSignIn;

    private ProgressDialog progressDialog;

    private FirebaseAuth firebaseAuth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_teacher);

        firebaseAuth = FirebaseAuth.getInstance();
/*
        if(firebaseAuth.getCurrentUser() != null){
            finish();
            startActivity(new Intent(getApplicationContext(),SaveTeacherDActivity.class));

        }*/

        progressDialog = new ProgressDialog(this);

        buttonRegister =(Button) findViewById(R.id.buttonRegister);

        editTextEmail =(EditText) findViewById(R.id.editTextEmail);
        editTextPassword =(EditText) findViewById(R.id.editTextPassword);

        textViewSignIn =(TextView) findViewById(R.id.textViewSignIn);

        buttonRegister.setOnClickListener(this);
        textViewSignIn.setOnClickListener(this);
    }

    private void registerUser(){
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

        progressDialog.setMessage("회원가입중입니다. 잠시만 기다려주세요");
        progressDialog.show();


       firebaseAuth.createUserWithEmailAndPassword(email,password)                      //firbase Auth에 이메일과 패스워드 등록과정
                    .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {

                        @Override
                        public void onComplete(@NonNull Task<AuthResult> task) {

                            if (task.isSuccessful()) {                  //성공적으로 등록되었다면
                                    finish();
                                    startActivity(new Intent(getApplicationContext(),DataUploadActivity.class));   //데이터 업로드 액티비티로

                            } else {
                                Toast.makeText(RegisterTeacherActivity.this, "등록할수 없습니다. 다시 시도해주세요", Toast.LENGTH_LONG).show();
                            }
                            progressDialog.dismiss();
                        }

                    });
    }

    @Override
    public void onClick(View view) {

        if(view==buttonRegister){
                registerUser();

        }

        if(view == textViewSignIn){

                startActivity(new Intent(this,LoginActivity.class));
            finish();
        }
    }
}
