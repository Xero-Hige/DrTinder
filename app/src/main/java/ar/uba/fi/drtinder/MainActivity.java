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
 * @author Xero-Hige
 * Copyright 2016 Gaston Martinez Gaston.martinez.90@gmail.com
 * <p>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/**
 * Main app activity of the app Dr Tinder.
 * Displays the 2 main fragments: Selection and Chatlist
 */
public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    /**
     * TODO
     */
    public static final String EXTRA_TOKEN = "token";
    private MenuItem mActualFragItem;
    private String mUsername;

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
        assert drawer != null;
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        assert navigationView != null; //DEBUG Assert
        navigationView.setNavigationItemSelectedListener(this);
        Fragment selectionFragment = new SelectionFragment();
        changeFragment(selectionFragment);
        mUsername = UserHandler.getUsername();
        Utility.hideKeyboard(this);
    }

    /**
     * TODO
     */
    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        assert drawer != null; //DEBUG Assert
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    /**
     * TODO
     *
     * @param menu
     * @return
     */
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

    /**
     * TODO
     *
     * @param item
     * @return
     */
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

        if (itemId == R.id.nav_profile) {
            Intent intent = new Intent(this, UserProfile.class);
            intent.putExtra(UserProfile.USER_EXTRA_USERNAME, mUsername);
            intent.putExtra(UserProfile.PROFILE_EXTRA_ACTION, UserProfile.PROFILE_ACTION_UPDATE);
            startActivity(intent);
        } else if (itemId == R.id.nav_logout) {
            UserHandler.logout();
            Intent intent = new Intent(this, LoginActivity.class);
            startActivity(intent);
            finish();
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        assert drawer != null; //DEBUG Assert
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    /**
     * TODO
     */
    @Override
    public void onDestroy() {
        super.onDestroy();
        ImageResourcesHandler.clearCache(this);
    }
}
