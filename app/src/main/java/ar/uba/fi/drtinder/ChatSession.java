package ar.uba.fi.drtinder;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.AppBarLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.widget.NestedScrollView;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 * Chat session activity. Represents a chat session between 2 users.
 */
public class ChatSession extends AppCompatActivity {

    private LinearLayout messagesLayout;

    private String yourId;
    private String friendId;
    private String friendName;

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.activity_chat_session);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent intent = getIntent();
        friendName = intent.getStringExtra("User");
        friendId = intent.getStringExtra("ID");
        yourId = "barrios"; //TODO: remove from here

        this.setTitle(friendName);

        messagesLayout = (LinearLayout) this.findViewById(R.id.messages);

        loadOldMessages();
        addSendListener();

        ImageView img = (ImageView) findViewById(R.id.backdrop);
        ImageResourcesHandler.fillImageResource(friendId, ImageResourcesHandler.RES_USER_IMG, img, this);

        FloatingActionButton scrollDownFB = (FloatingActionButton) this.findViewById(R.id.fab);
        assert scrollDownFB != null; //Debug assert
        scrollDownFB.setOnClickListener(listener -> lastMessageAnimation());
    }

    private void addSendListener() {
        ImageButton sendButton = (ImageButton) this.findViewById(R.id.send);
        assert sendButton != null; //Debug assert
        sendButton.setOnClickListener(
                listener -> {
                    EditText msgView = (EditText) findViewById(R.id.message);

                    assert msgView != null; //Debug assert
                    String message = msgView.getText().toString();

                    if (message.isEmpty()) {
                        return;
                    } else {

                        addPersonalResponse(message);
                        msgView.setText("");
                    }
                    lastMessageAnimation();
                });
    }

    private void loadOldMessages() {
        for (int i = 0; i < 24; i++) {
            addFriendResponse("Hola, como estas?");
        }
        addPersonalResponse("Veo que estas desesperada");
        addFriendResponse("no, te parece nomas.");
        addPersonalResponse("Bueno. Si pinta sadomasoquismo, avisame.");

        lastMessageAnimation();
    }

    private void addPersonalResponse(String message) {
        addResponse(R.layout.chat_session_you, "Tu", yourId, message);
    }

    private void addFriendResponse(String message) {
        addResponse(R.layout.chat_session_friend, friendName, friendId, message);
    }

    private void addResponse(int layoutId, String username, String userId, String message) {
        LayoutInflater inflater = LayoutInflater.from(this);
        View layout = inflater.inflate(layoutId, null);

        ImageView imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
        ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG, imageView, this);

        TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
        nameTextView.setText(username + ":");

        TextView msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
        msgTextView.setText(message);

        messagesLayout.addView(layout);
    }


    private void lastMessageAnimation() { //FIXME: change to a better name
        final NestedScrollView scrollview = ((NestedScrollView) findViewById(R.id.messages_lay));
        assert scrollview != null; //Debug assert
        scrollview.postDelayed(() -> scrollview.fullScroll(NestedScrollView.FOCUS_DOWN), 100);
        AppBarLayout appBarLayout = (AppBarLayout) findViewById(R.id.app_bar);
        assert appBarLayout != null; //Debug assert
        appBarLayout.setExpanded(false, true);
    }
}
