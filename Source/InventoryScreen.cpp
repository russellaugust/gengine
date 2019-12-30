//
// InventoryScreen.cpp
//
// Clark Kromenaker
//
#include "InventoryScreen.h"

#include "InventoryManager.h"
#include "RectTransform.h"
#include "UIButton.h"
#include "UICanvas.h"
#include "UIImage.h"

InventoryScreen::InventoryScreen() : Actor(TransformType::RectTransform)
{
	mCanvas = AddComponent<UICanvas>();
	
	// Add translucent background image that tints the scene.
	UIImage* background = AddComponent<UIImage>();
	mCanvas->AddWidget(background);
	background->SetTexture(Texture::Black);
	background->SetColor(Color32(0, 0, 0, 128));
	
	RectTransform* inventoryRectTransform = GetComponent<RectTransform>();
	inventoryRectTransform->SetSizeDelta(0.0f, 0.0f);
	inventoryRectTransform->SetAnchorMin(Vector2::Zero);
	inventoryRectTransform->SetAnchorMax(Vector2::One);
	
	// Add exit button to bottom-left corner of screen.
	Actor* exitButtonActor = new Actor(TransformType::RectTransform);
	UIButton* exitButton = exitButtonActor->AddComponent<UIButton>();
	mCanvas->AddWidget(exitButton);
	
	exitButton->SetUpTexture(Services::GetAssets()->LoadTexture("EXITN.BMP"));
	exitButton->SetDownTexture(Services::GetAssets()->LoadTexture("EXITD.BMP"));
	exitButton->SetHoverTexture(Services::GetAssets()->LoadTexture("EXITHOV.BMP"));
	exitButton->SetDisabledTexture(Services::GetAssets()->LoadTexture("EXITDIS.BMP"));
	exitButton->SetPressCallback(std::bind(&InventoryScreen::Hide, this));
	
	RectTransform* exitButtonRectTransform = exitButtonActor->GetComponent<RectTransform>();
	exitButtonRectTransform->SetParent(inventoryRectTransform);
	exitButtonRectTransform->SetSizeDelta(58.0f, 26.0f); // texture width/height
	exitButtonRectTransform->SetAnchor(Vector2::Zero);
	exitButtonRectTransform->SetAnchoredPosition(10.0f, 10.0f);
	exitButtonRectTransform->SetPivot(0.0f, 0.0f);
	
	// Hide inventory UI by default.
	Hide();
}

void InventoryScreen::Show(const std::set<std::string>& inventory)
{
	const float kStartX = 60.0f;
	const float kStartY = -90.0f;
	const float kSpacingX = 10.0f;
	const float kSpacingY = 10.0f;
	
	float x = kStartX;
	float y = kStartY;
	int counter = 0;
	for(auto& item : inventory)
	{
		// Only bother creating/positioning list item if we have a valid texture for it.
		//TODO: Use a placeholder/error texture?
		Texture* itemTexture = Services::Get<InventoryManager>()->GetInventoryItemListTexture(item);
		if(itemTexture == nullptr)
		{
			Services::GetReports()->Log("Error", "No inventory item texture found for " + item);
			continue;
		}
		
		// If this next item will go offscreen, we should move down to next row.
		if(x + itemTexture->GetWidth() > 1024.0f)
		{
			x = kStartX;
			y -= (itemTexture->GetHeight() + kSpacingY);
		}
		
		// Either reuse an already created button or create a new one.
		UIButton* button = nullptr;
		RectTransform* buttonRT = nullptr;
		if(counter < mItemButtons.size())
		{
			button = mItemButtons[counter];
			button->SetEnabled(true);
			
			buttonRT = button->GetOwner()->GetComponent<RectTransform>();
		}
		else
		{
			Actor* itemActor = new Actor(TransformType::RectTransform);
			button = itemActor->AddComponent<UIButton>();
			mCanvas->AddWidget(button);
			
			buttonRT = itemActor->GetComponent<RectTransform>();
			buttonRT->SetParent(GetComponent<RectTransform>());
			buttonRT->SetAnchor(Vector2(0.0f, 1.0f));
			buttonRT->SetPivot(0.0f, 1.0f);
		}
		
		// Set position/size for button.
		buttonRT->SetAnchoredPosition(x, y);
		buttonRT->SetSizeDelta(itemTexture->GetWidth(), itemTexture->GetHeight());
		
		// Set texture for button.
		button->SetUpTexture(itemTexture);
		
		// Next button located to the right, with spacing.
		x += itemTexture->GetWidth() + kSpacingX;
	}
	
	// Actually show the stuff!
	SetActive(true);
}

void InventoryScreen::Hide()
{
	SetActive(false);
}
