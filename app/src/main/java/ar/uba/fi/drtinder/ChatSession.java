package ar.uba.fi.drtinder;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.squareup.picasso.Picasso;

public class ChatSession extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat_session);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        Intent intent = getIntent();
        String text = intent.getStringExtra("User");
        this.setTitle(text);

        loadOldMessages();

        Button b = (Button) this.findViewById(R.id.send);
        b.setOnClickListener(
                v -> {
                    LinearLayout bar = (LinearLayout) this.findViewById(R.id.messages);
                    EditText msgView = (EditText) findViewById(R.id.message);
                    LayoutInflater inflater = LayoutInflater.from(this);
                    View layout = inflater.inflate(R.layout.chat_session_you, null);
                    ImageView imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
                    Picasso.with(bar.getContext()).load(
                            R.drawable.man).fit().centerCrop().into(imageView);

                    TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
                    nameTextView.setText("Tu:");

                    TextView msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
                    msgTextView.setText(msgView.getText());
                    msgView.setText("");
                    bar.addView(layout);
                });

    }

    private void loadOldMessages() {
        LinearLayout bar = (LinearLayout) this.findViewById(R.id.messages);
        LayoutInflater inflater = LayoutInflater.from(this);
        for (int i = 0; i < 16; i++) {

            View layout = inflater.inflate(R.layout.chat_session_friend, null);
            ImageView imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
            Picasso.with(bar.getContext()).load(
                    R.drawable.gato_5).fit().centerCrop().into(imageView);

            TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
            nameTextView.setText("Anastasia Steele dijo:");

            TextView msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
            msgTextView.setText("Hola, como estas?");

            bar.addView(layout);
        }

        View layout = inflater.inflate(R.layout.chat_session_you, null);
        ImageView imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
        Picasso.with(bar.getContext()).load(
                R.drawable.man).fit().centerCrop().into(imageView);

        TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
        nameTextView.setText("Tu:");

        TextView msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
        msgTextView.setText("Veo que estas desesperada.");

        bar.addView(layout);

        layout = inflater.inflate(R.layout.chat_session_friend, null);
        imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
        Picasso.with(bar.getContext()).load(
                R.drawable.gato_5).fit().centerCrop().into(imageView);

        nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
        nameTextView.setText("Anastasia Steele dijo:");

        msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
        msgTextView.setText("no, te parece nomas.");

        bar.addView(layout);

        layout = inflater.inflate(R.layout.chat_session_you, null);
        imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
        Picasso.with(bar.getContext()).load(
                R.drawable.man).fit().centerCrop().into(imageView);

        nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
        nameTextView.setText("Tu:");

        msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
        msgTextView.setText("Bueno. Si pinta sadomasoquismo, avisame.");

        bar.addView(layout);
    }
}
