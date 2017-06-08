package clean6265.com.naver.blog.login;

import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
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

import org.w3c.dom.Text;

import java.security.PublicKey;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

//이 액티비티에서 수정할 점은 직렬화로 받지 않아도 리스트뷰의 아이템들을 수정할 수 있어야 한다.
//즉, 데이터베이스에서 데이터들을 전부 긁어온 다음에, uuid, Level, Quest들을 리스트에 올릴 수 있어야 한다.
public class DataInBeaconActivity extends AppCompatActivity {

    private ListView mListView = null;
    private ListViewAdapter mAdapter = null;
    private AlertDialog mDialog = null, mSelectDialog = null, mModifyQuestDialog = null;

    private ArrayList<ListData> arrayList = new ArrayList<ListData>();
    private Uri filepath;
    private String FileType, beaconUUID;

    private static final int PICK_IMAGE_REQUEST =234;

    private FirebaseAuth firebaseAuth;
    private DatabaseReference mRef;
    private StorageReference storageReference;
    Thread readyThread =null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_data_in_beacon);
        storageReference = FirebaseStorage.getInstance().getReference();
/*
        readyThread = new Thread(new Runnable() {
            @Override
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mAdapter.notifyDataSetChanged();
                    }
                });
            }
        });
*/


        Intent intent = getIntent();
        //arrayList = (ArrayList) intent.getSerializableExtra("arrayList");
        //직렬화를 통해 전달된 인텐트이므로 받을 땐 역직렬화를 통해 받습니다.
        //역 직렬화는 getSerializableExtra를 통해 받고, 캐스팅을 합니다.
        //캐스팅을 하는 이유는 byte단위의 스트림을 통해 온 메모리형태를 객체가 요구하는
        //메모리의 형태로 다시 잡아주어야 하기 때문입니다.


        mListView = (ListView) findViewById(R.id.mList);

        mAdapter = new ListViewAdapter(this);
        mListView.setAdapter(mAdapter);





        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View v, int position, long id){
                ListData mData = (ListData)arrayList.get(position);
                beaconUUID = mData.mDate;   //uuid획득
                mDialog = createDialog();
                mDialog.show();
            }
        });

        mRef = FirebaseDatabase.getInstance().getReference("BeaconID"); //DataBase의 BeaconID의 하위항목의 전부를 참조하도록 합니다.

        mRef.addValueEventListener(new ValueEventListener() {   //firebase의 DataBase에서 정보들을 모두 가져오도록 합니다.
            //또한 이후에 값이 바뀌더라도 ValueEventListener에서 그것을 감지하여 onDataChange메소드를 호출합니다.
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                arrayList.clear();
                for(DataSnapshot postSnapshot : dataSnapshot.getChildren()){    //getChildren()메소드는 BeaconID의 하위 항목 전체를 iterator형식으로 한번씩 반환합니다.
                    //즉, BeaconID의 하위 항목 전체를 처음부터 끝까지 한번씩 반환하겠다는 의미입니다.
                    AnswerHintInformation temp = postSnapshot.getValue(AnswerHintInformation.class);
                    ListData data = new ListData();
                    data.mPri = temp.Pri;
                    data.mDate = postSnapshot.getKey(); //getKey()메소드는 해당 항목의 key값을 반환합니다.
                    data.mTitle = temp.Quest;

                    arrayList.add(data);

                }
                mAdapter.dataChange();

            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.e("DataBaseError!: ", "Please confirm the database");
            }



        });



    }




    private AlertDialog createDialog() {
        AlertDialog.Builder ab = new AlertDialog.Builder(this);

        ab.setTitle("기존의 파일을 수정하시겠습니까?");
        ab.setCancelable(true);


        ab.setPositiveButton("예", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                mModifyQuestDialog = createDialog(2);//함수 오버로딩으로 다른 함수를 호출하도록 합니다.
                mModifyQuestDialog.show();
                //파일의 uri를 이용하여 firebase에 데이터에 접근하여 수정할 수 있도록 합니다.
                //firebase의 디렉토리에 접근할때는 비콘의 uuid가 사용됩니다.
            }
        });
        ab.setNegativeButton("아니오", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                setDismiss(mDialog);
                finish();
            }
        });
        return ab.create();
    }

    private AlertDialog createDialog(int number){

        AlertDialog.Builder ab = new AlertDialog.Builder(this);
        if(number ==1 ) {
            ab.setTitle("어떤 형식의 파일을 올리시겠습니까?");
            ab.setCancelable(true);

            ab.setPositiveButton("이미지", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface arg0, int arg1) {
                    FileType = "image/*";
                    showFileChooser();
                    setDismiss(mSelectDialog);
                }
            });
            ab.setNegativeButton("동영상", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface arg0, int arg1) {
                    FileType = "video/*";
                    showFileChooser();
                    setDismiss(mSelectDialog);
                }
            });

        }
        else if(number ==2){
            ab.setTitle("문제 수정");
            ab.setMessage("문제의 수정을 원치 않으시면 취소를 눌러주세요.");

            final EditText input = new EditText(this);
            ab.setView(input);

            ab.setPositiveButton("수정", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    String value = input.getText().toString();
                    value.toString();


//                    mRef.child(beaconUUID).child("Quest").setValue(value);
                    mRef.child(beaconUUID).child("Quest").setValue(value);
                    mSelectDialog = createDialog(3);
                    mSelectDialog.show();

                    setDismiss(mModifyQuestDialog);
                    //            mAdapter.dataChange();



                }
            });

            ab.setNegativeButton("취소", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    mSelectDialog = createDialog(3);
                    mSelectDialog.show();
                    setDismiss(mModifyQuestDialog);
                }
            });
        }
        else if(number == 3){
            ab.setTitle("답안 수정");
            ab.setMessage("답안의 수정을 원치 않으시면 취소를 눌러주세요.");

            final EditText input = new EditText(this);
            ab.setView(input);

            ab.setPositiveButton("수정", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    String value = input.getText().toString();
                    value.toString();

                    mRef.child(beaconUUID).child("Ans").setValue(value);
                    mSelectDialog = createDialog(1);
                    mSelectDialog.show();
                }
            });

            ab.setNegativeButton("취소", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    mSelectDialog = createDialog(1);
                    mSelectDialog.show();
                }
            });
        }
        return ab.create();
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

                            finish();
                            Intent intent = new Intent(DataInBeaconActivity.this, DataInBeaconActivity.class);
                            startActivity(intent);
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


        }

    }
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {              //핸드폰에서 사진가져옴
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode==PICK_IMAGE_REQUEST && resultCode==RESULT_OK&&data!=null&&data.getData()!=null){
            filepath=data.getData();
            uploadFile();
        }
    }

    private void setDismiss(Dialog dialog){
        if(dialog != null && dialog.isShowing())
            dialog.dismiss();
    }

    private class ViewHolder{

        public ImageView mIcon;
        public TextView mTitle;
        public TextView mUUID;


    }

    private class ListViewAdapter extends BaseAdapter {
        private Context mContext = null;
        //private ArrayList<ListData> mListData = new ArrayList<ListData>();

        public ListViewAdapter(Context mContext) {
            super();
            this.mContext = mContext;

        }

        @Override
        public int getCount() {

            return arrayList.size();
        }

        @Override
        public Object getItem(int position) {

            return arrayList.get(position);
        }

        @Override
        public long getItemId(int position) {

            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ViewHolder holder;
            if (convertView == null) {
                holder = new ViewHolder();

                LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                convertView = inflater.inflate(R.layout.listview_item, null);

                holder.mIcon = (ImageView) convertView.findViewById(R.id.mImage);
                holder.mTitle = (TextView) convertView.findViewById(R.id.mTitle);
                holder.mUUID = (TextView) convertView.findViewById(R.id.mUUID);


                ListData data = (ListData)arrayList.get(position);
                holder.mTitle.setText(data.mTitle+"(Level : "+ String.valueOf(data.mPri)+")");
                holder.mUUID.setText(data.mDate );

                convertView.setTag(holder);
            }else{
                holder = (ViewHolder) convertView.getTag();
            }

            return convertView;

        }

        public void addItem(Drawable icon, String mTitle, String mDate, int mPri){
            ListData addInfo = null;
            addInfo = new ListData();
            // addInfo.mIcon = icon;
            addInfo.mTitle = mTitle;
            addInfo.mDate = mDate;
            addInfo.mPri = mPri;

            arrayList.add(addInfo);
        }

        public void remove(int position){
            arrayList.remove(position);
            dataChange();
        }

        public void sort(){
            Collections.sort(arrayList, ListData.ALPHA_COMPARATOR);
            dataChange();
        }

        public void dataChange(){
            mAdapter.notifyDataSetChanged();
        }



    }


}