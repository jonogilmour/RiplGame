﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "DirectXPage.xaml.h"




void ::RiplGame::DirectXPage::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///DirectXPage.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///DirectXPage.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Frame named 'MainMenu'
    MainMenu = safe_cast<::Windows::UI::Xaml::Controls::Frame^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"MainMenu"));
    // Get the SwapChainPanel named 'swapChainPanel'
    swapChainPanel = safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"swapChainPanel"));
    // Get the Grid named 'LayoutRoot'
    LayoutRoot = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"LayoutRoot"));
    // Get the TextBlock named 'txtX'
    txtX = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"txtX"));
    // Get the TextBlock named 'txtY'
    txtY = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"txtY"));
    // Get the TextBlock named 'txtZ'
    txtZ = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"txtZ"));
}

void ::RiplGame::DirectXPage::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::RiplGame::DirectXPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::Button_Click_1);
        break;
    case 2:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::RiplGame::DirectXPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::Button_Click);
        break;
    case 3:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::RiplGame::DirectXPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::Button_Click_2);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}

