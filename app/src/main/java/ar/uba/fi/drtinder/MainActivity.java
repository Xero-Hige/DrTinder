package ar.uba.fi.drtinder;

import android.content.Intent;
import android.graphics.PorterDuff;
import android.os.Bundle;
import android.support.design.widget.NavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;

/**
 * Main app activity of the app Dr Tinder. Displays the 2 main fragments
 */
public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private MenuItem mActualFragItem;

    private String username;

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open,
                R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        Fragment selectionFragment = new SelectionFragment();
        changeFragment(selectionFragment);
        username = UserInfoHandler.getUsername();
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        mActualFragItem = menu.findItem(R.id.action_swipe_pictures);
        mActualFragItem.setEnabled(false);
        PorterDuff.Mode mMode = PorterDuff.Mode.MULTIPLY;
        mActualFragItem.getIcon().setColorFilter(
                getResources().getColor(R.color.colorAccent), mMode);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int itemId = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (itemId == R.id.action_swipe_pictures) {

            changeItemColor(item);

            Fragment selectionFragment = new SelectionFragment();
            changeFragment(selectionFragment);
        }
        if (itemId == R.id.action_users_chat) {

            changeItemColor(item);

            Fragment chatsFragment = new ChatFragment();
            changeFragment(chatsFragment);
        }
        return super.onOptionsItemSelected(item);
    }

    private void changeFragment(Fragment selectionFragment) {
        FragmentManager frag = getSupportFragmentManager();
        frag.beginTransaction().replace(R.id.section_layout, selectionFragment).commit();
    }

    private void changeItemColor(MenuItem item) {
        mActualFragItem.setEnabled(true);
        PorterDuff.Mode mMode = PorterDuff.Mode.MULTIPLY;
        mActualFragItem.getIcon().setColorFilter(getResources().getColor(R.color.icons), mMode);
        mActualFragItem = item;
        mActualFragItem.setEnabled(false);
        mActualFragItem.getIcon().setColorFilter(
                getResources().getColor(R.color.accent), mMode);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int itemId = item.getItemId();

        if (itemId == R.id.nav_camera) {
            Snackdebug.showMessage("No te podes sacar fotos", findViewById(R.id.section_layout));
        } else if (itemId == R.id.nav_gallery) {
            Snackdebug.showMessage("No podes ver fotos", findViewById(R.id.section_layout));

        } else if (itemId == R.id.nav_slideshow) {
            Snackdebug.showMessage("Basta", findViewById(R.id.section_layout));

        } else if (itemId == R.id.nav_manage) {
            Intent intent = new Intent(this, UserProfile.class);
            intent.putExtra(UserProfile.USER_EXTRA_USERNAME, username);
            intent.putExtra(UserProfile.PROFILE_EXTRA_ACTION, UserProfile.PROFILE_ACTION_UPDATE);
            startActivity(intent);
        } else if (itemId == R.id.nav_share) {
            Snackdebug.showMessage("Compartir?", findViewById(R.id.section_layout));

        } else if (itemId == R.id.nav_send) {
            Snackdebug.showMessage("Enviar?", findViewById(R.id.section_layout));

        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        ImageResourcesHandler.clearCache(this);
    }
}
